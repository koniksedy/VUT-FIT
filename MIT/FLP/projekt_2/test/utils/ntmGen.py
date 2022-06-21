# ntmGrn.py
# Program generates nondeterministic turing machine.
# Project: Turing-machine (FLP project 2)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 24.04.2022

import sys
import random
from collections import defaultdict


class Ntm:
    def __init__(self, a, s, t, left, change, steps, verbose) -> None:
        """Turing machine initialization and generating.

        Args:
            a (int): alphabet size
            s (int): states count
            t (int): input tape length
            left (float): probability of left move
            change (float): probability of writing
            steps (int): turing machine steps
            verbose (boolead): If true, then method prints tape modifications.
        """
        self.alphabet = [chr(i) for i in range(97, 97+a)]
        self.states = [chr(i) for i in range(65, 91) if i != ord("S") and i != ord("F")][:s-2] + ["S", "F"]
        self.tape = ["#"] + list(random.choices(self.alphabet, k=t))
        self.left_p = left
        self.change_p = change
        self.steps = steps
        self.rules = self.__create_rules(verbose)

    def __str__(self) -> str:
        out = ""
        for s1 in self.rules:
            for l1 in self.rules[s1]:
                for (s2, l2) in self.rules[s1][l1]:
                    out += f"{s1} {l1} {s2} {l2}\n"
        tape = "".join(self.tape)
        out += f"{tape}"
        return out

    @staticmethod
    def __pick_state(possible_states, state, letter, rules):
        """Method picks next state. It tries to make as less nondeterministic
        transitions as possible.

        Args:
            possible_states (set): set of turing machine states
            state (str): current machine state
            letter (str): next machine letter
            rules (dict): machine rules

        Returns:
            str: state
        """
        states_by_letters = set()
        for src in possible_states:
            offset = 1 if src == state else 0
            if src in rules:
                if letter in rules[src]:
                    states_by_letters.add((src, len(rules[src][letter])+offset))
                else:
                    states_by_letters.add((src, 0+offset))
            else:
                states_by_letters.add((src, 0+offset))
        return sorted(states_by_letters, key=lambda x: x[1])[0][0]


    def __create_rules(self, verbose):
        """Creates rules for the instance of turing machine.

        Args:
            verbose (boolean): If true, then method prints tape modifications.

        Returns:
            dict : created rules
        """
        rules = defaultdict(lambda: defaultdict(set))
        steps_cnt = 0
        head = 0
        state = "S"
        move_dir = None
        move_step = 0
        tape = list(self.tape)
        possible_states = list(set(self.states).difference("F"))

        if verbose:
            print("".join(tape[:head] + [state] + tape[head:]), file=sys.stderr)

        while steps_cnt < self.steps:
            if move_step == 0:
                if random.random() < self.left_p:
                    move_dir = "L"
                    move_step = random.randint(0, head)
                else:
                    move_dir = "R"
                    move_step = random.randint(0, len(self.tape)+2)

            if move_step != 0:
                old_letter = tape[head]
                # new_state = random.choice(possible_states)
                if move_dir == "L":
                    head -= 1
                else:
                    if len(tape) == head+1:
                        tape.append("#")
                    head += 1

                new_state = self.__pick_state(possible_states, state, tape[head], rules)
                rules[state][old_letter].add((new_state, move_dir))
                state = new_state

                move_step -= 1
                steps_cnt += 1

                if verbose:
                    print("".join(tape[:head] + [state] + tape[head:]), file=sys.stderr)

            if random.random() < self.change_p:
                old_letter = tape[head]
                new_letter = random.choice(self.alphabet)
                new_state = self.__pick_state(possible_states, state, new_letter, rules)
                # new_state = random.choice(possible_states)
                tape[head] = new_letter

                rules[state][old_letter].add((new_state, new_letter))
                state = new_state

                steps_cnt += 1

                if verbose:
                    print("".join(tape[:head] + [state] + tape[head:]), file=sys.stderr)

        rules[state][tape[head]].add(("F", tape[head]))
        state = "F"

        if verbose:
            print("".join(tape[:head] + [state] + tape[head:]), file=sys.stderr)

        return rules


def main():
    # Print help if needed.
    if {"-h", "--help"}.intersection(sys.argv):
        print("USAGE:")
        print("gen.py alphabet_size states input_len left_prob(1-righ_prob) change_prob steps {--verbose}")
        sys.exit(0)

    # Testing and parsing of the program arguments.
    if len(sys.argv) != 7 and len(sys.argv) != 8:
        raise Exception("Bad number of program arguments.")

    try:
        alphabet = int(sys.argv[1])
        states = int(sys.argv[2])
        input_len = int(sys.argv[3])
        left_prob = float(sys.argv[4])
        change_prob = float(sys.argv[5])
        steps = int(sys.argv[6])
    except Exception as e:
        raise Exception("Bad program argument.") from e

    assert 0 < alphabet < 27
    assert 1 < states < 27
    assert 0 < input_len
    assert 0 <= left_prob <= 1
    assert 0 <= change_prob <= 1
    assert 0 <= steps

    verbose = False
    if len(sys.argv) == 8:
        if sys.argv[7] != "--verbose":
            raise Exception(f"Unknown program argument {sys.argv[7]}.")
        verbose = True

    # Generate and print turing machine.
    ntm = Ntm(alphabet, states, input_len, left_prob, change_prob, steps, verbose)
    print(ntm)


if __name__ == "__main__":
    main()
