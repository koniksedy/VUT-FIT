"""
parser.py
DiGraph parsing module.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 31.10.2022
"""

import numpy as np
from bidict import bidict


class DiGraph:
    """
    Attributes:
        vertices (set[int]): A set of renamed (id of the vertex) graph states.
        vertices_cnt (int): Cont of graph vertices.
        vertex_cname (bidict): Two way dictionary with id (key) of vertex and original name (value).
        edges_mx (np.array): Two dimensional boolean incident matrix.
    """
    def __init__(self) -> None:
        self.vertices = set()
        self.vertices_cnt = 0
        self.vertex_cname = bidict()
        self.edges_mx = np.array([], dtype=bool)

    def __eq__(self, __o: object) -> bool:
        if not isinstance(self, __o):
            return False
        return self.edges_mx == __o.edges_mx

    def __len__(self) -> int:
        return self.vertices_cnt

    def __str__(self) -> str:
        out = ""
        for u in range(self.vertices_cnt):
            for v in range(self.vertices_cnt):
                if self.edges_mx[u][v]:
                    out += f"{self.vertex_cname[u]} {self.vertex_cname[v]}\n"
        return out

    def __repr__(self) -> str:
        return str(self.edges_mx)

    def add_vertex(self, vertex: str) -> None:
        """Adds new vertex to the graph.

        Args:
            vertex (str): Vertex.
        """
        if vertex not in self.vertex_cname.inverse:
            # Add vertex
            vertex_id = self.vertices_cnt
            self.vertex_cname[vertex_id] = vertex
            self.vertices.add(vertex_id)
            self.vertices_cnt += 1

            # Add new row and column to incident matrix.
            if self.edges_mx.size == 0:
                self.edges_mx = np.array([[False]], dtype=bool)
            else:
                row = np.array([[False] * (self.vertices_cnt -1)], dtype=bool)
                column = np.array([[False]] * self.vertices_cnt, dtype=bool)
                self.edges_mx = np.append(self.edges_mx, row, axis=0)
                self.edges_mx = np.append(self.edges_mx, column, axis=1)

    def add_edge(self, src: int, dst: int) -> None:
        """Adds edge between two vertices of the graph.

        Args:
            src (int): Id of the source vertex of the edge.
            dst (int): Id of the destination vertex of the edge.

        Raises:
            ValueError: The vertex does not exist.
        """
        if src not in self.vertices or dst not in self.vertices:
            raise ValueError("Unknown vertex.")

        if not self.edges_mx[src][dst]:
            self.edges_mx[src][dst] = True

    def prune_single_scc(self) -> None:
        """Recursively removes vertices with 0 input degree or 0 output degree.
        """
        if self.vertices_cnt == 0:
            return
        pruned = set()
        while True:
            # Finds empty columns or rows
            row_idxs = np.argwhere(np.all(self.edges_mx[..., :] == False, axis=0)).flatten()
            columns_idxs = np.argwhere(np.all(self.edges_mx[..., :] == False, axis=1)).flatten()
            to_prune = set(row_idxs).union(columns_idxs)

            if to_prune.issubset(pruned):
                break
            pruned.update(to_prune)

            # Hide edges
            for v_id in to_prune:
                self.edges_mx[v_id] = False
                self.edges_mx[:, v_id] = False

        # Remove edges
        for v_id in sorted(pruned, reverse=True):
            self.edges_mx = np.delete(self.edges_mx, v_id, axis=0)
            self.edges_mx = np.delete(self.edges_mx, v_id, axis=1)

        # Rename remaining states
        self.vertices_cnt -= len(pruned)
        self.vertices = set(range(self.vertices_cnt))
        new_canon = bidict()
        i = 0
        for v_id in self.vertex_cname:
            if v_id not in pruned:
                new_canon[i] = self.vertex_cname[v_id]
                i += 1
        self.vertex_cname = new_canon


    def get_successors(self, vertex_id: int) -> np.array:
        """Returns successors of the vertex.

        Args:
            vertex_id (int): Vertex

        Returns:
            np.array: Successors of the vertex.
        """
        return np.argwhere(self.edges_mx[vertex_id] == True).flatten()
