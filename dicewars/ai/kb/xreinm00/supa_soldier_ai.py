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
    params['epsilon_decay_linear'] = 1/100
    params['learning_rate'] = 0.00013629
    params['first_layer_size'] = 200        # neurons in the first layer
    params['second_layer_size'] = 60        # neurons in the second layer
    params['third_layer_size'] = 30         # neurons in the third layer
    params['episodes'] = 100   
    params['memory_size'] = 400
    params['batch_size'] = 200
    # Settings
    params['weights_path'] = os.path.join(os.getcwd(), 'dicewars/ai/kb/xreinm00/weights/weights.h5')
    params['train'] = True
    params['load_weights'] = False
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
        self.fresh_init = True
        self.player_areas_old = 1

        self.bad_prediction = False

        self.num_of_turns = 0
        self.num_of_model_predictions = 0
        self.num_of_bad_predictions = 0

        # loss values
        self.loss_vals = []
        self.epoch_loss= []


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
            self.player_areas_old = self.config['players_areas_old']
            self.loss_vals = self.config['loss_vals']
            print("Load weights: {}".format(self.load_weights))
        

        print("Episodes: {}, Games total: {}".format(self.params['episodes'], self.counter_games))
        
        """
        if len(self.loss_vals) == 8:
            import matplotlib
            matplotlib.use('Agg')
            import matplotlib.pyplot as plt
            print("Printing loss graph")
            temp = list(range(0, len(self.loss_vals)))
            plt.plot(np.linspace(1, temp, temp).astype(int), self.loss_vals)
            plt.savefig('mygraph.png')
        """ 

    def __del__(self):
        self.save_ai_state()
        print("Total turns: {}, Model made {} predictions, {} of them were bad.".format(self.num_of_turns, self.num_of_model_predictions, self.num_of_bad_predictions))
        if self.params['episodes'] == self.counter_games:
            if self.params['train']:
                model_weights = self.state_dict()
                torch.save(model_weights, self.params["weights_path"])
                print("Deleting state files...")
                os.remove(os.path.join(os.getcwd(), 'dicewars/ai/kb/xreinm00/pickles/SUPA_SOLDIER_STATE.pickle'))
            

    def ai_turn(self, board: Board, nb_moves_this_turn, nb_transfers_this_turn, nb_turns_this_game, time_left):
        self.optimizer = optim.Adam(self.parameters(), weight_decay=0, lr=self.params['learning_rate'])
        # self.optimizer = optim.SGD(self.parameters(), weight_decay=0, lr=self.params['learning_rate'])

        if self.fresh_init:
            self.fresh_init = False
            self.player_areas_old = board.get_player_areas(self.player_name)
            if self.params['train'] and len(self.memory) > 0:
                self.replay_new(self.memory, self.params['batch_size'])
        


        if not self.params['train']:
            self.epsilon = 0.01
        else:
            self.epsilon = 1 - (self.counter_games * self.params['epsilon_decay_linear'])

        # get results of previous round
        state_new, attacks = self.get_state(board)

        self.player_areas_current = board.get_player_areas(self.player_name)

        # get old state + attacks
        state_old, attacks = self.get_state(board)
        
        if len(attacks) == 0 or self.performed_attacks >= 10:
            self.performed_attacks = 0
            # print("Ending turn...")
            return EndTurnCommand()

        self.state_old = state_old
        
        NN_predicted = False
        self.bad_prediction = False
        # perform random actions based on agent.epsilon, or choose the action
        # print("Epsilon: ", self.epsilon)
        if random.uniform(0, 1) < self.epsilon:
            final_move = np.eye(6)[random.randint(0, len(attacks) - 1 if len(attacks) - 1 <= 5 else 5)]
            self.final_move = final_move
        else:
            # predict action based on the old state
            with torch.no_grad():
                NN_predicted = True
                state_old_tensor = torch.tensor(state_old.reshape((1, 36)), dtype=torch.float32).to(DEVICE)
                prediction = self(state_old_tensor)
                # print("Attacks len: ", len(attacks))
                # print("ASD", prediction.detach().cpu().numpy()[0])
                # print("MAX", np.argmax(prediction.detach().cpu().numpy()[0]))
                final_move = np.eye(6)[np.argmax(prediction.detach().cpu().numpy()[0])]
                self.final_move = final_move
                self.num_of_model_predictions += 1
                # print("NN Output: {}, attacks len: {}".format(prediction.detach().cpu().numpy()[0], len(attacks)))
                if len(attacks) <= np.argmax(prediction.detach().cpu().numpy()[0]):
                    
                    max_prob = 0
                    max_prob_index = 0
                    for i, attack in enumerate(attacks):
                        prob = probability_of_successful_attack(board, attack[0].get_name(), attack[1].get_name())
                        if max_prob < prob:
                            max_prob = prob
                            max_prob_index = i

                    final_move = np.eye(6)[max_prob_index]
                    self.final_move = final_move
                    self.bad_prediction = True
                    self.num_of_bad_predictions += 1
                    
        

        # print("Setting reward and training short memory....")
        # set reward for the new state
        reward = self.set_reward(len(self.player_areas_current), len(self.player_areas_old))

        if self.params['train']:
            # train short memory base on the new action and state
            self.train_short_memory(self.state_old, self.final_move, reward, state_new)
            # store the new data into a long term memory
            self.remember(self.state_old, self.final_move, reward, state_new)


        # perform new move and get new state
        index = 0
        for i, item in enumerate(final_move):
            if item == 1:
                index = i
                break
        
        src_target = attacks[index]
        # print("[{}] Attack: {} ({}) -> {} ({}), prob. of success: {} {}".format("NN" if NN_predicted else "RD", src_target[0].get_name(), src_target[0].get_dice(), src_target[1].get_name(), src_target[1].get_dice(), probability_of_successful_attack(board, src_target[0].get_name(), src_target[1].get_name()), "[Bad prediction]" if self.bad_prediction else ""))
        self.player_areas_old = board.get_player_areas(self.player_name)
        self.num_of_turns += 1
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
                "load_weights": self.load_weights,
                "players_areas_old": self.player_areas_old,
                "loss_vals": self.loss_vals
            }
            print("Saving state, weights: {}".format(self.load_weights))
            pickle.dump(state, config_dictionary_file)

    def load_ai_state(self):
        with open(os.path.join(os.getcwd(), 'dicewars/ai/kb/xreinm00/pickles/SUPA_SOLDIER_STATE.pickle'), 'rb') as config_dictionary_file:
            config_dictionary = pickle.load(config_dictionary_file)
            return config_dictionary if config_dictionary else None

    def network(self):
        # Layers
        self.f1 = nn.Linear(36, self.first_layer)
        self.f2 = nn.Linear(self.first_layer, self.second_layer)
        self.f3 = nn.Linear(self.second_layer, self.third_layer)
        self.f4 = nn.Linear(self.third_layer, 6)
        # weights
        if self.load_weights:
            self.load_weights = False
            self.model = self.load_state_dict(torch.load(self.weights))
            print("weights loaded, now its {}".format(self.load_weights))

    def forward(self, x):
        x = F.relu(self.f1(x))
        x = F.relu(self.f2(x))
        x = F.relu(self.f3(x))
        x = F.softmax(self.f4(x), dim=-1)
        return x
    
    def get_state(self, board: Board):
        state = []
        attacks = list(possible_attacks(board, self.player_name))
        attacks_sorted = sorted(attacks, key=lambda x: probability_of_successful_attack(board, x[0].get_name(), x[1].get_name()), reverse=True)
        if attacks:
            for i, attack in enumerate(attacks_sorted): 
                if i == 6: break
                src: Area = attack[0]
                dst: Area = attack[1]
                prob_of_success = probability_of_successful_attack(board, src.get_name(), dst.get_name())
                prob_of_hodl = probability_of_holding_area(board, src.get_name(), src.get_dice(), self.player_name)
                areas_around: list[int] = src.get_adjacent_areas_names()
                
                max_dice_around_target = 0
                for area_name in dst.get_adjacent_areas_names():
                    target_area: Area = board.get_area(area_name)
                    if target_area.get_owner_name() != self.player_name:
                        if max_dice_around_target < target_area.get_dice():
                            max_dice_around_target = target_area.get_dice()

                num_of_enemies_around = 0
                for area in areas_around:
                    if board.get_area(area).get_owner_name() != self.player_name: 
                        num_of_enemies_around += 1
                
                state.append(prob_of_success)
                state.append(prob_of_hodl)
                state.append(num_of_enemies_around)
                state.append(max_dice_around_target)
                state.append(src.get_dice())
                state.append(dst.get_dice())
                

                # print("""
                # Training dato:
                # ({})   Src dice:       {},
                # ({})   Target dice:    {},
                #        Enemies around: {},
                #        Success prob.:  {},
                #        Hodl prob.   :  {}
                #  """.format(src.get_name(), src.get_dice(), dst.get_name(), dst.get_dice(), num_of_enemies_around, prob_of_success, prob_of_hodl))

        for i in range(len(state), 36, 6):
            state.append(0.00)
            state.append(1.00)
            state.append(1)
            state.append(8)
            state.append(2)
            state.append(8)


        return np.asarray(state), attacks_sorted

    def set_reward(self, num_of_areas: int, last_num_of_areas: int):
        
        self.reward = 0
        if self.bad_prediction:
            self.reward = -5
        elif num_of_areas < last_num_of_areas:
            self.reward = -5
        elif num_of_areas > last_num_of_areas:
            self.reward = 10

        # print("Reward: {}".format(self.reward))
        # print("current areas: {}, old areas: {}".format(num_of_areas, last_num_of_areas))
        return self.reward

    def remember(self, state, action, reward, next_state):
        """
        Store the <state, action, reward, next_state, is_done> tuple in a 
        memory buffer for replay memory.
        """
        self.memory.append((state, action, reward, next_state))

    def replay_new(self, memory, batch_size):
        """
        Replay memory.
        """
        print("Starting replaying memory, mem len: {}".format(len(memory)))
        if len(memory) > batch_size:
            minibatch = random.sample(memory, batch_size)
        else:
            minibatch = memory

        for state, action, reward, next_state in minibatch:
            self.train()
            torch.set_grad_enabled(True)
            target = reward
            next_state_tensor = torch.tensor(np.expand_dims(next_state, 0), dtype=torch.float32).to(DEVICE)
            state_tensor = torch.tensor(np.expand_dims(state, 0), dtype=torch.float32, requires_grad=True).to(DEVICE)
            target = reward + self.gamma * torch.max(self.forward(next_state_tensor)[0])
            output = self.forward(state_tensor)
            target_f = output.clone()
            target_f[0][np.argmax(action)] = target
            target_f.detach()
            self.optimizer.zero_grad()
            loss = F.mse_loss(output, target_f)
            loss.backward()
            self.epoch_loss.append(loss.item())
            self.optimizer.step()    

        self.loss_vals.append(sum(self.epoch_loss)/len(self.epoch_loss))
        print("Loss value {}".format(self.loss_vals[-1]))
        print("Replay finished")       

    def train_short_memory(self, state, action, reward, next_state):
        """
        Train the DQN agent on the <state, action, reward, next_state, is_done>
        tuple at the current timestep.
        """
        self.train()
        torch.set_grad_enabled(True)
        target = reward
        next_state_tensor = torch.tensor(next_state.reshape((1, 36)), dtype=torch.float32).to(DEVICE)
        state_tensor = torch.tensor(state.reshape((1, 36)), dtype=torch.float32, requires_grad=True).to(DEVICE)
        target = reward + self.gamma * torch.max(self.forward(next_state_tensor[0]))
        output = self.forward(state_tensor)
        target_f = output.clone()
        target_f[0][np.argmax(action)] = target
        target_f.detach()
        self.optimizer.zero_grad()
        loss = F.mse_loss(output, target_f)
        loss.backward()
        # print("Loss value {}".format(loss.item()))
        self.optimizer.step()
