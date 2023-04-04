"""parkmeans.cc
The sequential implementation of K-means clustering algorithm for K=4.
Author: Michal Šedý <xsedym02@vutbr.cz>
Last change: 04.04.2023
"""


import sys
from struct import unpack


class Kmeans:
    """A class for clusters.

    Attributes:
        points (dict): A dictionary of points and specific cluster id, indexed by points.
        means (dict): A list of means for each cluster.
    """
    def __init__(self) -> None:
        self.points = dict()
        self.means = list()

    def __str__(self) -> str:
        out = ""
        for midx, vs in self.get_clusters().items():
            m = self.means[midx]
            out += f"[{round(m, 3)}] "
            for v in vs:
                out += f"{v[1]} "
            out += "\n"
        return out[:-1]

    def load_numbers(self, file_name: str, max_num=32, k=4) -> None:
        """Loads numbers from binary file.

        Args:
            file_name (str): An input file name.
            max_num (int, optional): The maximum number of input numbers. Defaults to 32.
            k (int, optional): A umber of clusters. Defaults to 4.
        """
        cnt = 0
        with open(file_name, "rb") as fh:
            while True:
                data = fh.read(1)
                if not data:
                    return
                if cnt > max_num:
                    return
                d = unpack("b", data)[0]
                if cnt < k:
                    self.means.append(d)
                self.points[(cnt, d)] = None
                cnt += 1

    def clean_means(self) -> None:
        """Removes duplicit means.
        """
        means = list()
        for m in self.means:
            if m not in means:
                means.append(m)
        self.means = means

    def recalculate_position(self, point: int) -> bool:
        """Recalculates the affiliation of the point to clusters.

        Args:
            point (int): Examined point.

        Returns:
            bool: If true, then the point changed cluster.
        """
        _, point_data = point
        min_idx, min_distance = 0, sys.maxsize
        for idx, mean in enumerate(self.means):
            distance = abs(point_data - mean)
            if distance < min_distance:
                min_idx = idx
                min_distance = distance

        old_idx = self.points[point]
        self.points[point] = min_idx

        return old_idx != self.points[point]

    def recalculate_means(self) -> None:
        """Calculates new cluster means.
        """
        for midx, ps in self.get_clusters().items():
            new_m = sum(map(lambda x: x[1], ps))/len(ps)
            self.means[midx] = new_m

    def compute(self) -> None:
        """Computers K-means.
        """
        changed = True
        while changed:
            changed = False
            for point in self.points:
                changed = self.recalculate_position(point) or changed
            self.recalculate_means()

    def get_clusters(self) -> dict:
        """Returns inverted self.points.

        Returns:
            dict: Dictionary of clusters and points, indexed by clusters id.
        """
        dict_by_mean = dict()
        for point, midx in self.points.items():
            if midx not in dict_by_mean:
                dict_by_mean[midx] = set()
            dict_by_mean[midx].add(point)
        return dict_by_mean


def main() -> None:
    kmeans = Kmeans()
    kmeans.load_numbers(sys.argv[1])
    kmeans.clean_means()
    kmeans.compute()
    print(kmeans)


if __name__ == "__main__":
    main()
