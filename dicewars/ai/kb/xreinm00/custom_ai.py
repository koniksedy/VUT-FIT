import random
import logging
import pickle
import os
import pprint

from dicewars.ai.utils import attack_succcess_probability, possible_attacks, probability_of_holding_area, probability_of_successful_attack

from dicewars.client.ai_driver import BattleCommand, EndTurnCommand, TransferCommand


class CustomAI:
    def __init__(self, player_name, board, players_order, max_transfers):
        self.player_name = player_name
        self.logger = logging.getLogger('AI')
        print("I've been initialized: {}".format(self.player_name))
        self.config = self.load_ai_state()
    
    def __del__(self):
        self.save_ai_state()
    
    def save_ai_state(self):
        with open(os.path.join(os.getcwd(), 'dicewars/ai/kb/xreinm00/pickles/config.dictionary'), 'wb') as config_dictionary_file:
            pickle.dump(self.config, config_dictionary_file)

    def load_ai_state(self):
        with open(os.path.join(os.getcwd(), 'dicewars/ai/kb/xreinm00/pickles/config.dictionary'), 'rb') as config_dictionary_file:
            config_dictionary = pickle.load(config_dictionary_file)
            config_dictionary['remote_port'] += 1
            return config_dictionary if config_dictionary else {'remote_hostname': 'google.com', 'remote_port': 69}

    def ai_turn(self, board, nb_moves_this_turn, nb_transfers_this_turn, nb_turns_this_game, time_left):
        # self.print_gamestate(board, nb_moves_this_turn, nb_transfers_this_turn, nb_turns_this_game, time_left)

        if nb_moves_this_turn == 6:
            self.logger.info("I'm too well behaved. Let others play now.")
            return EndTurnCommand()

        attacks = list(possible_attacks(board, self.player_name))
        if attacks:
            source, target = random.choice(attacks)
            return BattleCommand(source.get_name(), target.get_name())
        else:
            self.logger.debug("No more possible turns.")
            return EndTurnCommand()

    def print_gamestate(self, board, nb_moves_this_turn, nb_transfers_this_turn, nb_turns_this_game, time_left):
        if nb_moves_this_turn == 0: return
        # possible_attcks = possible_attacks(board, self.player_name)
        print(
            """
            --- TURN {} --- (AI: {})
            Number of transfers this turn: {}
            Number of turn this game: {}
            Time left: {}
            """.format(nb_moves_this_turn, self.player_name, nb_transfers_this_turn, nb_turns_this_game, time_left))
        
        print("Possible attacks:")
        for attack in list(possible_attacks(board, self.player_name)):
            print("My area: {} ({}), Enemy area: {} ({})".format(attack[0].get_name(), attack[0].get_dice(), attack[1].get_name(), attack[1].get_dice()))
            attack_success = probability_of_successful_attack(board, attack[0].get_name(), attack[1].get_name())
            hodl_prob = probability_of_holding_area(board, attack[0].get_name(), attack[0].get_dice(), attack[0].get_owner_name())
            print("Attack success prob.: {}".format(attack_success))
            print("Holding prob.: {}".format(hodl_prob))
