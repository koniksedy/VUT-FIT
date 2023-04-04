import sys


class Kmeans:
    def __init__(self) -> None:
        self.points = dict()
        self.means = list()

    def __str__(self) -> str:
        out = ""
        for midx, vs in self.get_clusters().items():
            m = self.means[midx]
            out += f"[{round(m, 3)}] "
            for v in vs:
                out += f"{v} "
            out += "\n"
        return out[:-1]

    def load_numbers(self, file_name: str, max_num=32, k=4) -> None:
        cnt = 0
        with open(file_name, "rb") as fh:
            while True:
                data = list(fh.readline())
                if not data:
                    return
                for d in data:
                    if cnt > max_num:
                        return
                    if cnt < k:
                        self.means.append(d)
                    self.points[d] = None
                    cnt += 1

    def recalculate_position(self, point: int) -> bool:
        min_idx, min_distance = 0, 1000000
        for idx, mean in enumerate(self.means):
            distance = abs(point - mean)
            if distance < min_distance:
                min_idx = idx
                min_distance = distance

        old_idx = self.points[point]
        self.points[point] = min_idx

        return old_idx != self.points[point]

    def recalculate_means(self) -> None:
        for midx, ps in self.get_clusters().items():
            new_m = sum(ps)/len(ps)
            self.means[midx] = new_m

    def compute(self) -> None:
        changed = True
        while changed:
            changed = False
            for point in self.points:
                changed = self.recalculate_position(point) or changed
            self.recalculate_means()

    def get_clusters(self) -> dict:
        dict_by_mean = dict()
        for d, midx in self.points.items():
            if midx not in dict_by_mean:
                dict_by_mean[midx] = set()
            dict_by_mean[midx].add(d)
        return dict_by_mean


def main() -> None:
    kmeans = Kmeans()
    kmeans.load_numbers(sys.argv[1])
    print(f"INPUT NUMBERS: {' '.join([str(d) for d in kmeans.points.keys()])}")
    kmeans.compute()
    print(kmeans)


if __name__ == "__main__":
    main()
