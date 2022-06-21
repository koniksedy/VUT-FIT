# validator.py
# Program validates the solution of a nondeterministic turing machine.
# Project: Turing-machine (FLP project 2)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 24.04.2022

import sys
from collections import defaultdict
from curses.ascii import islower, isupper


class Ntm:
    def __init__(self) -> None:
        self.state = "S"
        self.head = 0
        self.tape = list()
        self.rules = defaultdict(lambda: defaultdict(set))
        self.step_cnt = 1

    def load_rules(self, file_name: str):
        """Load Turing machine rules and the input tape from a given file.

        Args:
            file_name (str): file with a turing machine

        Raises:
            e: Bad file format
        """
        fd = open(file_name, "r")
        lines = fd.readlines()
        tape = "S" + lines[-1].rstrip()
        self.tape = list(tape)
        for line in lines[:-1]:
            line = line.rstrip()
            if not line:
                continue
            try:
                s1, l1, s2, l2 = line.split(" ")
            except Exception as e:
                print(tape)
                print(line.split(" "))
                raise e
            self.rules[s1][l1].add((s2, l2))

    def next_tape(self, tape):
        """Test that the Turing machine configuration has been reached correctly.

        Args:
            tape (list): Turing machine tape.
        """
        def cmp_tapes(t1, t2):
            if not all(map(lambda x: islower(x) or x == "#", t1)) or not all(map(lambda x: islower(x) or x == "#", t2)):
                return False
            return t1 == t2

        s1 = self.state
        l1 = self.tape[self.head+1]
        s2 = ""
        l2 = ""

        new_head = self.head
        test_tape1 = self.tape[:self.head] + self.tape[self.head+2:]

        if self.head != 0 and isupper(tape[self.head-1]):
            l2 = "L"
            s2 = tape[self.head-1]
            test_tape2 = tape[:self.head-1] + [tape[self.head]] + tape[self.head+2:]
            new_head -= 1
        elif isupper(tape[self.head]):
            l2 = tape[self.head+1]
            s2 = tape[self.head]
            test_tape2 = tape[:self.head] + tape[self.head+2:]
        elif isupper(tape[self.head+1]):
            l2 = "R"
            s2 = tape[self.head+1]
            if self.head+2 == len(self.tape):
                test_tape2 = tape[:-3]
            else:
                test_tape2 = tape[:self.head] + tape[self.head+2:]
            new_head += 1
        else:
            print("DIFF: State is not on the expected possition or on the tape.", file=sys.stderr)
            print(f"Mismatch between {self.step_cnt} and {self.step_cnt+1} step.", file=sys.stderr)
            print(f"{self.step_cnt}:{''.join(self.tape)}\n{self.step_cnt+1}:{''.join(tape)}", file=sys.stderr)
            return False

        if s1 not in self.rules or l1 not in self.rules[s1] or (s2, l2) not in self.rules[s1][l1]:
            print(f"DIFF: No rule '{s1} {l1} {s2} {l2}' for the tape modification.", file=sys.stderr)
            print(f"Mismatch between {self.step_cnt} and {self.step_cnt+1} step.", file=sys.stderr)
            print(f"{self.step_cnt}:{''.join(self.tape)}\n{self.step_cnt+1}:{''.join(tape)}", file=sys.stderr)
            return False

        if not cmp_tapes(test_tape1, test_tape2):
            print("DIFF: More states in the tape or tape modification withou head on.", file=sys.stderr)
            print(f"Mismatch between {self.step_cnt} and {self.step_cnt+1} step.", file=sys.stderr)
            print(f"{self.step_cnt}:{''.join(self.tape)}\n{self.step_cnt+1}:{''.join(tape)}", file=sys.stderr)
            return False

        self.tape = tape
        self.head = new_head
        self.state = self.tape[new_head]
        self.step_cnt += 1

        return True


class Validator:
    def __init__(self, input_file, rules_file) -> None:
        self.ntm = Ntm()
        self.ntm.load_rules(rules_file)
        self.cmpt_steps = list()
        with open(input_file, "r") as fd:
            for line in fd.readlines():
                self.cmpt_steps.append(list(line.rstrip()))

    def validate(self):
        if "S" not in self.cmpt_steps[0]:
            print("DIFF: No initial state S.", file=sys.stderr)
            print(f"Expected:{''.join(self.ntm.tape)}\nPROLOG:{''.join(self.cmpt_steps[0])}", file=sys.stderr)
            return False

        if "F" not in self.cmpt_steps[-1]:
            print("DIFF: No final state F.", file=sys.stderr)
            print(f"PROLOG:{''.join(self.cmpt_steps[-1])}", file=sys.stderr)
            return False

        if self.ntm.tape != self.cmpt_steps[0]:
            print("DIFF: Input tape differ in rules and output.", file=sys.stderr)
            print(f"ORIG:{''.join(self.ntm.tape)}\nPROLOG:{''.join(self.cmpt_steps[0])}", file=sys.stderr)
            return False

        for step in self.cmpt_steps[1:]:
            if not self.ntm.next_tape(step):
                return False

        return True


def main():
    if {"-h", "--help"}.intersection(sys.argv):
        print("USAGE:")
        print("validator.py test_input rules")
        sys.exit(0)

    if len(sys.argv) != 3:
        raise Exception("Bad number of program parameters.")

    validator = Validator(sys.argv[1], sys.argv[2])
    if not validator.validate():
        sys.exit(1)


if __name__ == "__main__":
    main()
