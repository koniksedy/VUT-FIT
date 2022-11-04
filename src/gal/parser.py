"""
parser.py
DiGraph parsing module.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 31.10.2022
"""
from src.gal.digraph import DiGraph
from src.gal.algorithms import get_cycles


class Parser:

    @staticmethod
    def parse(file_name: str) -> DiGraph:
        """Parses DiGraph from a given file and returns its instance.
        File structure:
        v1 v2       // edge from v1 to v2
        v3 v4

        Args:
            file_name (str): Input file with DiGraph.

        Returns:
            DiGraph: Parsed graph.
        """
        graph = DiGraph()
        with open(file_name, "r") as fd:
            for line in fd.readlines():
                line = line.strip()
                if not line:
                    continue
                src, dst = line.split()
                graph.add_vertex(src)
                graph.add_vertex(dst)
                graph.add_edge(graph.vertex_cname.inverse[src], graph.vertex_cname.inverse[dst])

        return graph


def main():
    import sys
    g = Parser.parse(sys.argv[1])
    g.prune_single_scc()
    get_cycles(g, "book")
    print(g)


if __name__ == "__main__":
    main()
