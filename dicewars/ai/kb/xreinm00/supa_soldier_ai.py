import random
import numpy as np
import pandas as pd
from operator import add
import collections
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import copy
import datetime
import os
import pickle
import time

from dicewars.ai.utils import possible_attacks, probability_of_holding_area, probability_of_successful_attack
from dicewars.client.ai_driver import BattleCommand, EndTurnCommand
from dicewars.client.game.area import Area
from dicewars.client.game.board import Board
DEVICE = 'cuda' if torch.cuda.is_available() else 'cpu'

def define_parameters():
    params = dict()
    # Neural Network
    params['epsilon_decay_linear'] = 1/30
    params['learning_rate'] = 0.00013629
    params['first_layer_size'] = 200        # neurons in the first layer
    params['second_layer_size'] = 20        # neurons in the second layer
    params['third_layer_size'] = 50         # neurons in the third layer
    params['episodes'] = 100        
    params['memory_size'] = 2500
    params['batch_size'] = 100
    # Settings
    params['weights_path'] = os.path.join(os.getcwd(), 'dicewars/ai/kb/xreinm00/weights/weights.h5')
    params['train'] = True
    params['load_weights'] = True
    params['log_path'] = 'logs/scores_' + str(datetime.datetime.now().strftime("%Y%m%d%H%M%S")) +'.txt'
    return params

class DQNSupaSoldierAI(torch.nn.Module):
    def __init__(self, player_name, board, players_order, max_transfers):
        super().__init__()
        self.player_name = player_name
        self.params = define_parameters()
        self.reward = 0
        self.gamma = 0.9
        self.dataframe = pd.DataFrame()
        self.short_memory = np.array([])
        self.agent_target = 1
        self.agent_predict = 0
        self.learning_rate = self.params['learning_rate']        
        self.epsilon = 1
        self.actual = []
        self.first_layer = self.params['first_layer_size']
        self.second_layer = self.params['second_layer_size']
        self.third_layer = self.params['third_layer_size']
        self.memory = collections.deque(maxlen=self.params['memory_size'])
        self.weights = self.params['weights_path']
        self.load_weights = self.params['load_weights']
        self.counter_games = 0
        self.performed_attacks = 0

        self.config = self.load_ai_state()
        self.network()
        
        if self.config:
            print("Config loaded")
            self.epsilon = self.config['epsilon']
            self.reward = self.config['reward']
            self.short_memory = self.config['short_memory']
            self.memory = self.config['memory']
            self.counter_games = self.config['counter_games']
            self.load_weights = self.config['load_weights']
        
        

    def __del__(self):
        print("Episodes on Netflix: {}, current game num: {}".format(self.params['episodes'], self.counter_games))

        # if self.params['train']:
        #    self.replay_new(self.memory, self.params['batch_size'])
        self.save_ai_state()

    def ai_turn(self, board: Board, nb_moves_this_turn, nb_transfers_this_turn, nb_turns_this_game, time_left):
        
        # self.optimizer = optim.Adam(self.parameters(), weight_decay=0, lr=self.params['learning_rate'])
        self.optimizer = optim.SGD(self.parameters(), weight_decay=0, lr=self.params['learning_rate'])

        if nb_turns_this_game == 0:
            if self.params['train']:
                self.replay_new(self.memory, self.params['batch_size'])
            
            if self.params['episodes'] == self.counter_games + 1:
                if self.params['train']:
                    model_weights = self.state_dict()
                    torch.save(model_weights, self.params["weights_path"])


        if not self.params['train']:
            self.epsilon = 0.01
        else:
            self.epsilon = 1 - (self.counter_games * self.params['epsilon_decay_linear'])

        # get results of previous round
        state_new, attacks = self.get_state(board)

        self.player_areas_current = board.get_player_areas(self.player_name)

        # get old state + attacks
        state_old, attacks = self.get_state(board)
        if len(attacks) == 0 or self.performed_attacks >= 6:
            self.performed_attacks = 0
            # print("Ending turn...")
            return EndTurnCommand()

        if nb_turns_this_game != 0:
            # set reward for the new state
            reward = self.set_reward(len(self.player_areas_current), len(self.player_areas_old))

            if self.params['train']:
                # train short memory base on the new action and state
                self.train_short_memory(self.state_old, self.final_move, reward, state_new, True if len(attacks) == 0 else False)
                # store the new data into a long term memory
                self.remember(self.state_old, self.final_move, reward, state_new, True if len(attacks) == 0 else False)



        self.state_old = state_old

        NN_predicted = False
        # perform random actions based on agent.epsilon, or choose the action
        # print("Epsilon: ", self.epsilon)
        if random.uniform(0, 1) < self.epsilon:
            final_move = np.eye(6)[random.randint(0, len(attacks) - 1 if len(attacks) - 1 <= 5 else 5)]
            self.final_move = final_move
        else:
            # predict action based on the old state
            with torch.no_grad():
                NN_predicted = True
                state_old_tensor = torch.tensor(state_old.reshape((1, 30)), dtype=torch.float32).to(DEVICE)
                prediction = self(state_old_tensor)
                # print("Attacks len: ", len(attacks))
                # print("ASD", prediction.detach().cpu().numpy()[0])
                # print("MAX", np.argmax(prediction.detach().cpu().numpy()[0]))
                final_move = np.eye(6)[np.argmax(prediction.detach().cpu().numpy()[0])]
                self.final_move = final_move
                
                if len(attacks) <= np.argmax(prediction.detach().cpu().numpy()[0]):
                    final_move = np.eye(6)[len(attacks)-1]

        self.player_areas_old = board.get_player_areas(self.player_name)
        
        # perform new move and get new state
        index = 0
        for i, item in enumerate(final_move):
            if item == 1:
                index = i
                break
        
        try:
            src_target = attacks[index]
        except Exception as e:
            print("Attacks len: {}".format(len(attacks)))
            print("Counted index: {}".format(index))
            print("final move: {}".format(final_move))
            print("Attacking target, predicted by NN: {}".format(NN_predicted))
        src_target = attacks[index]
        self.performed_attacks += 1
        return BattleCommand(src_target[0].get_name(), src_target[1].get_name())  
    
    def save_ai_state(self):
        print("Saving current model state...")
        with open(os.path.join(os.getcwd(), 'dicewars/ai/kb/xreinm00/pickles/SUPA_SOLDIER_STATE.pickle'), 'wb') as config_dictionary_file:
            self.counter_games += 1
            state = {
                "epsilon": 1 - (self.counter_games * self.params['epsilon_decay_linear']),
                "reward": self.reward,
                "short_memory": self.short_memory,
                "memory": self.memory,
                "counter_games": self.counter_games,
                "load_weights": self.load_weights
            }
            pickle.dump(state, config_dictionary_file)

    def load_ai_state(self):
        with open(os.path.join(os.getcwd(), 'dicewars/ai/kb/xreinm00/pickles/SUPA_SOLDIER_STATE.pickle'), 'rb') as config_dictionary_file:
            config_dictionary = pickle.load(config_dictionary_file)
            return config_dictionary if config_dictionary else None

    def network(self):
        # Layers
        self.f1 = nn.Linear(30, self.first_layer)
        self.f2 = nn.Linear(self.first_layer, self.second_layer)
        self.f3 = nn.Linear(self.second_layer, self.third_layer)
        self.f4 = nn.Linear(self.third_layer, 6)
        # weights
        if self.load_weights:
            self.load_weights = False
            self.model = self.load_state_dict(torch.load(self.weights))
            print("weights loaded")

    def forward(self, x):
        x = F.relu(self.f1(x))
        x = F.relu(self.f2(x))
        x = F.relu(self.f3(x))
        x = F.softmax(self.f4(x), dim=-1)
        return x
    
    def get_state(self, board: Board):
        state = []
        attacks = list(possible_attacks(board, self.player_name))
        if attacks:
            for i, attack in enumerate(attacks): 
                if i == 6: break
                src: Area = attack[0]
                dst: Area = attack[1]
                prob_of_success = probability_of_successful_attack(board, src.get_name(), dst.get_name())
                prob_of_hodl = probability_of_holding_area(board, src.get_name(), src.get_dice(), self.player_name)
                areas_around: list[int] = src.get_adjacent_areas_names()
                num_of_enemies_around = 0
                for area in areas_around:
                    if board.get_area(area).get_owner_name() != self.player_name: 
                        num_of_enemies_around += 1
                
                state.append(prob_of_success)
                state.append(prob_of_hodl)
                state.append(num_of_enemies_around)
                state.append(src.get_dice())
                state.append(dst.get_dice())

        for i in range(len(state), 30):
            state.append(0)

        return np.asarray(state), attacks

        """
        Return the state.
        The state is a numpy array of 11 values, representing:
            - Danger 1 OR 2 steps ahead
            - Danger 1 OR 2 steps on the right
            - Danger 1 OR 2 steps on the left
            - Snake is moving left
            - Snake is moving right
            - Snake is moving up
            - Snake is moving down
            - The food is on the left
            - The food is on the right
            - The food is on the upper side
            - The food is on the lower side      
    
        state = [
            (player.x_change == 20 and player.y_change == 0 and ((list(map(add, player.position[-1], [20, 0])) in player.position) or
            player.position[-1][0] + 20 >= (game.game_width - 20))) or (player.x_change == -20 and player.y_change == 0 and ((list(map(add, player.position[-1], [-20, 0])) in player.position) or
            player.position[-1][0] - 20 < 20)) or (player.x_change == 0 and player.y_change == -20 and ((list(map(add, player.position[-1], [0, -20])) in player.position) or
            player.position[-1][-1] - 20 < 20)) or (player.x_change == 0 and player.y_change == 20 and ((list(map(add, player.position[-1], [0, 20])) in player.position) or
            player.position[-1][-1] + 20 >= (game.game_height-20))),  # danger straight

            (player.x_change == 0 and player.y_change == -20 and ((list(map(add,player.position[-1],[20, 0])) in player.position) or
            player.position[ -1][0] + 20 > (game.game_width-20))) or (player.x_change == 0 and player.y_change == 20 and ((list(map(add,player.position[-1],
            [-20,0])) in player.position) or player.position[-1][0] - 20 < 20)) or (player.x_change == -20 and player.y_change == 0 and ((list(map(
            add,player.position[-1],[0,-20])) in player.position) or player.position[-1][-1] - 20 < 20)) or (player.x_change == 20 and player.y_change == 0 and (
            (list(map(add,player.position[-1],[0,20])) in player.position) or player.position[-1][
             -1] + 20 >= (game.game_height-20))),  # danger right

             (player.x_change == 0 and player.y_change == 20 and ((list(map(add,player.position[-1],[20,0])) in player.position) or
             player.position[-1][0] + 20 > (game.game_width-20))) or (player.x_change == 0 and player.y_change == -20 and ((list(map(
             add, player.position[-1],[-20,0])) in player.position) or player.position[-1][0] - 20 < 20)) or (player.x_change == 20 and player.y_change == 0 and (
            (list(map(add,player.position[-1],[0,-20])) in player.position) or player.position[-1][-1] - 20 < 20)) or (
            player.x_change == -20 and player.y_change == 0 and ((list(map(add,player.position[-1],[0,20])) in player.position) or
            player.position[-1][-1] + 20 >= (game.game_height-20))), #danger left


            player.x_change == -20,  # move left
            player.x_change == 20,  # move right
            player.y_change == -20,  # move up
            player.y_change == 20,  # move down
            food.x_food < player.x,  # food left
            food.x_food > player.x,  # food right
            food.y_food < player.y,  # food up
            food.y_food > player.y  # food down
        ]

        for i in range(len(state)):
            if state[i]:
                state[i]=1
            else:
                state[i]=0

        return np.asarray(state)
        """

    def set_reward(self, num_of_areas: int, last_num_of_areas: int):
        self.reward = 0

        if num_of_areas < last_num_of_areas:
            self.reward = -10
        elif num_of_areas > last_num_of_areas:
            self.reward = 50

        return self.reward

    def remember(self, state, action, reward, next_state, done):
        """
        Store the <state, action, reward, next_state, is_done> tuple in a 
        memory buffer for replay memory.
        """
        self.memory.append((state, action, reward, next_state, done))

    def replay_new(self, memory, batch_size):
        """
        Replay memory.
        """
        print("Starting replaying memory, mem len: {}".format(len(memory)))
        if len(memory) > batch_size:
            minibatch = random.sample(memory, batch_size)
        else:
            minibatch = memory

        for state, action, reward, next_state, done in minibatch:
            self.train()
            torch.set_grad_enabled(True)
            target = reward
            next_state_tensor = torch.tensor(np.expand_dims(next_state, 0), dtype=torch.float32).to(DEVICE)
            state_tensor = torch.tensor(np.expand_dims(state, 0), dtype=torch.float32, requires_grad=True).to(DEVICE)
            if not done:
                target = reward + self.gamma * torch.max(self.forward(next_state_tensor)[0])
            output = self.forward(state_tensor)
            target_f = output.clone()
            target_f[0][np.argmax(action)] = target
            target_f.detach()
            self.optimizer.zero_grad()
            loss = F.mse_loss(output, target_f)
            loss.backward()
            self.optimizer.step()    

        print("Replay finished")       

    def train_short_memory(self, state, action, reward, next_state, done):
        """
        Train the DQN agent on the <state, action, reward, next_state, is_done>
        tuple at the current timestep.
        """
        self.train()
        torch.set_grad_enabled(True)
        target = reward
        next_state_tensor = torch.tensor(next_state.reshape((1, 30)), dtype=torch.float32).to(DEVICE)
        state_tensor = torch.tensor(state.reshape((1, 30)), dtype=torch.float32, requires_grad=True).to(DEVICE)
        if not done:
            target = reward + self.gamma * torch.max(self.forward(next_state_tensor[0]))
        output = self.forward(state_tensor)
        target_f = output.clone()
        target_f[0][np.argmax(action)] = target
        target_f.detach()
        self.optimizer.zero_grad()
        loss = F.mse_loss(output, target_f)
        loss.backward()
        self.optimizer.step()
