"""randomgen.py
Generates random data for Huffman coding/decoding using "random" different values.
Author: Michal Šedý <xsedym02@vutbr.cz>
Last change: 04.05.2023
"""


import random
from argparse import ArgumentParser


def parse_args():
    parser = ArgumentParser()
    parser.add_argument("--width", type=int, required=True)
    parser.add_argument("--height", type=int, required=True)
    parser.add_argument("-r", "--random", type=int, required=True)
    parser.add_argument("-o", "--output", type=str, required=True)

    args = parser.parse_args()

    if args.width < 1:
        raise AttributeError("A width must be positive.")
    if args.height < 1:
        raise AttributeError("A height must be positive.")
    if not (0 < args.random < 257):
        raise AttributeError("A random must be within interval [1, 256]")

    return parser.parse_args()

def save(data, file_name):
    with open(file_name, "wb") as fh:
        fh.write(bytes(data))


def main():
    args = parse_args()
    size = args.width * args.height

    values = list(range(args.random))
    data = list()
    for _ in range(size):
        data.append(random.choice(values))

    save(data, args.output)


if __name__ == "__main__":
    main()
