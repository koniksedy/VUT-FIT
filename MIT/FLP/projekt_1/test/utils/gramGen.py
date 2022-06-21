# gramGrn.py
# Program generates Context Free Grammar of given properties.
# The output of the program are three files:
#   *-i.cfg    with original grammar
#   *-1.cfg    grammar withou nongenerating nonterminals
#   *-2.cfg    grammar withou unnecessary (unreachable + nongenerating) symbols
# Project: SIMPLIFY-BKG (FLP)
# Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 24.03.2022

import sys
from random import randint, random, choice, choices
from copy import deepcopy

class CFG:
    def __init__(self) -> None:
        self.nonterms = set()
        self.terms = set()
        self.start = ""
        self.rules = list()

    def __str__(self) -> str:
        """ Return the grammar in the format:
        (nonterminals)
        (terminals)
        (start nonterminal)
        (rule)
        (rule)
        (...)
        """
        out = ",".join(sorted(self.nonterms)) + "\n"
        out += ",".join(sorted(self.terms.difference({"#"}))) + "\n"
        out += self.start + "\n"
        for src, dsts in self.rules:
            out += f"{src}->{dsts}\n"
        return out[:-1]

    def random(self, nts_cnt, ts_cnt, rules_per_nts, max_right, epsilon_prob) -> None:
        """Initialize random grammar.

        Nonterminals are upper letters except the letter "Z".
        Terminals are all lower letters.
        The start sybol is reseved letter "Z".

        Args:
            nts_cnt (int): Number of nonterminals
            ts_cnt (int): Number of terminals
            rules_per_term (float): Number of rules per nonterminal. Duplicit rules are ommited.
            max_right (int): The maximum number of symbols on the right side of a rule.
                             The actual size of the right side is determined by the uniform distribution.
            epsilon_prob (float): The probability that the rule will be an epsilon rule.
        """
        nontermsList = [chr(i) for i in range(65, 64+nts_cnt)] + ["Z"]
        termsList = [chr(i) for i in range(97, 97+ts_cnt)]
        self.nonterms = set(nontermsList)
        self.terms = set(termsList).union("#")
        self.start = "Z"
        self.rules = [("Z", "#")]

        symbols = nontermsList + termsList
        for _ in range(int(rules_per_nts*nts_cnt)):
            left = choice(nontermsList)
            if random() < epsilon_prob and (left, "#") not in self.rules:

                self.rules.append((left, "#"))
            else:
                right = "".join(choices(symbols, k=randint(1, max_right)))
                if left == right or (left, right) in self.rules:
                    continue
                self.rules.append((left, right))

    def __genNonterms(self) -> set:
        nonterms = set()
        while True:
            newNonterms = {src for (src, dsts) in self.rules if set(dsts).issubset(nonterms.union(self.terms))}
            if newNonterms == nonterms:
                break
            nonterms = newNonterms
        return nonterms

    def __reachable(self) -> set:
        reachable = {self.start}
        while True:
            newReachable = {dst for (src, dsts) in self.rules for dst in dsts if src in reachable}.union(reachable)
            if reachable == newReachable:
                break
            reachable = newReachable
        return reachable


    def minimize(self, heavy):
        """Removes unnecessary symbols from grammar and corresponding rules.

        Args:
            heavy (bool): If False, only non-mercer symbols will be removed,
                          otherwise, all unnecessary symbols are removed.
        """
        generating = self.__genNonterms()
        self.nonterms = generating.union({self.start})
        self.rules = [(src, dsts) for (src, dsts) in self.rules if src in generating and set(dsts).issubset(generating.union(self.terms))]

        if not heavy:
            return

        reachable = self.__reachable()
        self.nonterms = generating.intersection(reachable)
        self.terms = self.terms.intersection(reachable)
        self.rules = [(src, dsts) for (src, dsts) in self.rules if src in self.nonterms and set(dsts).issubset(reachable)]


def parse_opts(argv):
    if "-h" in argv or "--help" in argv:
        print("USAGE:", file=sys.stderr)
        print("python3 testGen.py nonterm_cnt term_cnt reles_per_nonterm max_righ_side epsilon_prob output", file=sys.stderr)
        sys.exit(0)

    if len(argv) != 6:
        print("Program arguments error. Try --help.", file=sys.stderr)
        exit(1)

    try:
        nts_cnt = int(argv[0])
        ts_cnt = int(argv[1])
        rs_cnt = float(argv[2])
        max_r = int(argv[3])
        epsilion_prob = float(argv[4])
        output = argv[5]
    except ValueError:
        print("Bad type of program parameter. Try --help.", file=sys.stderr)

    assert 0 <= nts_cnt < 26, "Too many/few nonterminals (max 25, min 0)."
    assert 0 < ts_cnt <= 26, "Too many/few terminals (max 26, min 0)."
    assert 0 < rs_cnt, "Too few rules per nonterminal (> 0)."
    assert 0 < max_r, "Too show right side of a rule (min 1)."
    assert 0 < epsilion_prob < 1, "Epsilon probbability must be from the range (0, 1)."

    return nts_cnt, ts_cnt, rs_cnt, max_r, epsilion_prob, output


def main():
    grammar = CFG()
    opts = parse_opts(sys.argv[1:])
    output = opts[-1]
    grammar.random(*opts[:-1])

    # Print original grammar.
    with open(f"{output}-i.cfg", "w") as fd:
        print(grammar, file=fd)

    # Print grammar withou nongenerating symbols.
    light_grammar = deepcopy(grammar)
    light_grammar.minimize(False)
    with open(f"{output}-1.cfg", "w") as fd:
        print(light_grammar, file=fd)

    # Print grammar without unnecessary symbols.
    grammar.minimize(True)
    with open(f"{output}-2.cfg", "w") as fd:
        print(grammar, file=fd)

if __name__ == "__main__":
    main()
