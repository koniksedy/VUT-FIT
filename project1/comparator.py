import sys


def load_numbers(file_name: str) -> list:
    with open(file_name, "rb") as fh:
        return list(fh.readline())

def numbers_from_string(string: str) -> list:
    numbers = list()
    for n in string.split("[")[1].split("]")[0].split(","):
        if n != "":
            numbers.append(int(n))
    return numbers

def load_results() -> tuple:
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

    print_result(lt, eq, gt)

if __name__ == "__main__":
    main()
