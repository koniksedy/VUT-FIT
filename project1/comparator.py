""" Tests parsplit.cc output using sequential split.
Author: Michal Šedý <xsedym02@vutbr.cz>
Last change: 30.03.2023
"""


import sys


def load_numbers(file_name: str) -> list:
    """Loads numbers from the parsplit.cc's input file.

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

def numbers_from_string(string: str) -> list:
    """Parses numbers from parsplit.cc's output.
    Output format: 'GT = [1,2]'

    Args:
        string (str): A string being parsed.

    Returns:
        list: A list of obtained numbers.
    """
    numbers = list()
    for n in string.split("[")[1].split("]")[0].split(","):
        if n.isalnum():
            numbers.append(int(n))
    return numbers

def load_results() -> tuple:
    """Loads parsplit.cc's output from stdin.

    Returns:
        tuple: A triple of splitted numbers using parsplit.cc.
    """
    lt = numbers_from_string(sys.stdin.readline())
    eq = numbers_from_string(sys.stdin.readline())
    gt = numbers_from_string(sys.stdin.readline())
    return lt, eq, gt

def print_result(lt, eq, gt) -> None:
    print("LT = [", end="")
    if lt:
        print(",".join([str(n) for n in lt]), end="")
    print("]")

    print("EQ = [", end="")
    if eq:
        print(",".join([str(n) for n in eq]), end="")
    print("]")

    print("GT = [", end="")
    if gt:
        print(",".join([str(n) for n in gt]), end="")
    print("]")

def seqsplit(numbers: list) -> tuple:
    """Sequential split.

    Args:
        numbers (list): A list of numbers being splitted.

    Returns:
        tuple: A triple of splitted numbers.
    """
    mid = 0
    if len(numbers) % 2 == 0:
        mid = numbers[int(len(numbers)/2)-1]
    else:
        mid = numbers[int(len(numbers)/2)]

    lt = list()
    eq = list()
    gt = list()

    for n in numbers:
        if n < mid:
            lt.append(n)
        elif n == mid:
            eq.append(n)
        else:
            gt.append(n)

    return lt, eq, gt


def main():
    numbers = load_numbers(sys.argv[1])
    lt, eq, gt = seqsplit(numbers)

    lt_c, eq_c, gt_c = load_results()

    try:
        assert set(lt) == set(lt_c)
        assert set(eq) == set(eq_c)
        assert set(gt) == set(gt_c)
    except AssertionError as e:
        print_result(lt, eq, gt)
        print("#"*80)
        print_result(lt_c, eq_c, gt_c)
        raise e


if __name__ == "__main__":
    main()
