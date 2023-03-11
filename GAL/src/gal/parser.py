"""
parser.py
DiGraph parsing module.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 15.11.2022
"""

from .digraph import DiGraph


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
