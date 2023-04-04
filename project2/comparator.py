import sys
from seqkmeans import Kmeans


def get_clusters_from_stdin() -> dict:
    data = dict()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        mean = round(float(line.split("[")[1].split("]")[0]), 1)
        data[mean] = set()
        for v in line.split()[1:]:
            v = v.strip()
            if v:
                data[mean].add(int(v))
    return data

def main() -> None:
    kmeans = Kmeans()
    kmeans.load_numbers("numbers")
    kmeans.clean_means()
    kmeans.compute()
    tmp = kmeans.get_clusters()
    output1 = dict()
    for k, vs in tmp.items():
        output1[round(kmeans.means[k], 1)] = {v for _, v in vs}

    output2 = get_clusters_from_stdin()

    if output1 != output2:
        print(output1)
        print(output2)

    assert output1 == output2

if __name__ == "__main__":
    main()
