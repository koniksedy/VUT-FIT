import sys
import random


def main() -> None:
    stations = {"A", "B", "C", "D"}
    max_weight = int(sys.argv[3])
    max_time = int(sys.argv[2])
    used_time = set()
    for i in range(int(sys.argv[1])):
        src = random.choice(list(stations))
        dst = random.choice(list(set(stations).difference({src})))
        weight = random.randint(1, max_weight)

        time = random.randint(0, max_time)
        while time in used_time:
            time = random.randint(0, max_time)

        print(time, src, dst, weight, f"cargo-{i}", sep=",")


if __name__ == "__main__":
    main()
