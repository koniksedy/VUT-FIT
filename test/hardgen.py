"""hardgen.py
Generated hard data for Huffman coder/decoder testing.
Hard means that the difference between two neighbor pixels is 255.
Author: Michal Šedý <xsedym02@vutbr.cz>
Last change: 04.05.2023
"""

import random
from argparse import ArgumentParser


def parse_args():
    parser = ArgumentParser()
    parser.add_argument("--width", type=int, required=True)
    parser.add_argument("--height", type=int, required=True)
    parser.add_argument("-r", "--random", action="store_true", default=False)
    parser.add_argument("-o", "--output", type=str, required=True)

    args = parser.parse_args()

    if args.width < 1:
        raise AttributeError("A width must be positive.")
    if args.height < 1:
        raise AttributeError("A height must be positive.")

    return parser.parse_args()

def get_next_val(val, rand):
    if rand:
        if random.random() < 0.5:
            return 0
        else:
            return 255
    else:
        if val == 0:
            return 255
        else:
            return 0

def save(data, file_name):
    with open(file_name, "wb") as fh:
        fh.write(bytes(data))


def main():
    args = parse_args()

    data = list()
    val_i = 0
    for _ in range(args.height):
        val_j = val_i
        for _ in range(args.width):
            data.append(val_j)
            val_j = get_next_val(val_j, args.random)
        val_i = get_next_val(val_i, args.random)

    save(data, args.output)


if __name__ == "__main__":
    main()
