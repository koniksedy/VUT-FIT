"""Calculates entropy of a file.
Author: Michal Šedý <xsedym02@vutbr.cz>
Last change: 05.05.2023
"""

import sys
from math import log2
from collections import defaultdict


def load_numbers(file_name: str) -> list:
    """Loads numbers from the parsplit.cpp's input file.
    Args:
        file_name (str): A binary file with numbers to split.
    Returns:
        list: A list of loaded numbers.
    """
    data_out = list()
    with open(file_name, "rb") as fh:
        while True:
            data = list(fh.readline())
            if not data:
                break
            else:
                data_out.extend(data)
    return list(data_out)


def main():
    numbers = load_numbers(sys.argv[1])

    n_cnt = defaultdict(int)
    for n in numbers:
        n_cnt[n] += 1

    prob = defaultdict(float)
    for n in n_cnt.keys():
        if n_cnt[n]:
            prob[n] = n_cnt[n]/len(numbers)

    sum = 0
    for n in prob.keys():
        sum += prob[n]*log2(prob[n])

    print(sys.argv[1], "=", round(-sum, 2))


if __name__ == "__main__":
    main()
