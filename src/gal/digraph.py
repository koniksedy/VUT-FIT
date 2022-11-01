"""
digraph.py
DiGraph module.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 01.11.2022
"""

import numpy as np
import random
from bidict import bidict


class DiGraph:
    """
    Attributes:
        vertices (set[int]): A set of renamed (id of the vertex) graph states.
        vertices_cnt (int): Cont of graph vertices.
        vertex_cname (bidict): Two-way dictionary with id (key) of vertex and original name (value).
        edges_mx (np.array): Two dimensional boolean incident matrix.
    """
    def __init__(self, graph=None) -> None:
        """DiGraph constructor.

        Args:
            graph (DiGraph, optional): If not None, then the graph will be created as a copy.
                                       Defaults to None.
        """
        if graph is None:
            self.vertices = set()
            self.vertices_cnt = 0
            self.vertex_cname = bidict()
            self.edges_mx = np.array([], dtype=bool)
        else:
            self.vertices = set(graph.vertices)
            self.vertices_cnt = graph.vertices_cnt
            self.vertex_cname = bidict(graph.vertex_cname)
            self.edges_mx = np.copy(graph.edges_mx)

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

    @staticmethod
    def create_complete_graph(vertices_cnt: int):
        """Creates complete graph.

        Args:
            vertices_cnt (int): Number of vertices in the complete graph.

        Returns:
            DiGraph: Complete graph.
        """
        graph = DiGraph()
        graph.vertices_cnt = vertices_cnt
        graph.vertices = set(range(graph.vertices_cnt))
        graph.vertex_cname = bidict({v: v for v in graph.vertices})
        graph.edges_mx = np.ones((graph.vertices_cnt, graph.vertices_cnt), dtype=bool)
        return graph

    @staticmethod
    def create_multicycle_graph(vertices_cnt: int, additional_edges_cnt: int):
        """Create cyclic graph with additional edges.

        Args:
            vertices_cnt (int): Number to vertices.
            additional_edges_cnt (int): Number to additional edges.

        Returns:
            DiGraph: Multi cycle graph.
        """
        graph = DiGraph()
        graph.set_vertices(set(range(vertices_cnt)))
        # Create cyclic graph
        for u in graph.vertices:
            v = (u + 1) % graph.vertices_cnt
            graph.edges_mx[u][v] = True
        # Add additional edges
        possible_uv = np.argwhere(graph.edges_mx == False)
        edges_cnt = min(possible_uv.size // 2, additional_edges_cnt)
        uv = random.sample(possible_uv.tolist(), edges_cnt)
        for (u, v) in uv:
            graph.edges_mx[u][v]= True

        return graph


    def set_vertices(self, vertices: set) -> None:
        """Prepare graph for given vertices. The previous graph data will be deleted.

        Args:
            vertices (set): Vertices
        """
        self.vertices_cnt = len(vertices)
        self.vertices = set(vertices)
        self.vertex_cname = bidict({i: v for i, v in zip(self.vertices, vertices)})
        self.edges_mx = np.zeros((self.vertices_cnt, self.vertices_cnt), dtype=bool)

    def add_vertex(self, vertex: str) -> None:
        """Adds new vertex to the graph.

        Args:
            vertex (str): Vertex.
        """
        if vertex not in self.vertex_cname.inverse:
            # Add vertex
            v_id = self.vertices_cnt
            self.vertex_cname[v_id] = vertex
            self.vertices.add(v_id)
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

    def transpose(self):
        """Creates transposed graph.

        Returns:
            DiGraph: Transposed graph.
        """
        graph = DiGraph()
        graph.vertices = self.vertices
        graph.vertices_cnt = self.vertices_cnt
        graph.vertex_cname = self.vertex_cname
        graph.edges_mx = self.edges_mx.transpose()
        return graph

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
        for v_id in sorted(self.vertex_cname.keys()):
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

    def get_induced_graph(self, vertices: set):
        """Creates induced graph based on the set of given vertices.

        Args:
            vertices (set): Vertices of the new graph.

        Returns:
            DiGraph: Induced graph.
        """
        graph = DiGraph()
        if not vertices:
            return graph

        graph.vertices_cnt = len(vertices)
        graph.vertices = set(range(graph.vertices_cnt))
        graph.edges_mx = np.zeros((graph.vertices_cnt, graph.vertices_cnt), dtype=bool)

        # Rebase edges and cname
        new_ids = {v_id: new_id for v_id, new_id in zip(vertices, graph.vertices)}
        for u in vertices:
            graph.vertex_cname[new_ids[u]] = self.vertex_cname[u]
            for v in vertices:
                graph.edges_mx[new_ids[u]][new_ids[v]] = self.edges_mx[u][v]

        return graph

    def dfs(self, order_key = None, order_reverse = False) -> tuple:
        """Performs DFS.

        Args:
            order_key (function, optional): Function according to which the root vertices are selected
                                            (in the main for cycle). If None, then vertices are chosen
                                            randomly. Defaults to None.
            order_reverse (bool, optional): If True, then the root vertices are selected in
                                            descending order. Otherwise, the ascending order is applied.
                                            Defaults to False.

        Returns:
            tuple[list, list, list]: The method returns the list of trees (a set of vertices) in the DFS
                                     forest, the list of d times (times of the beginning of a work on
                                     vertices), and the list of f times (times of the end of a work on vertices).
        """
        def dfs_visit(s) -> set:
            nonlocal time
            visited = {s}
            colors[s] = GRAY
            time += 1
            d[s] = time
            for v in self.get_successors(s):
                if colors[v] == WHITE:
                    visited.update(dfs_visit(v))
            colors[s] = BLACK
            time += 1
            f[s] = time
            return visited

        WHITE = 0
        GRAY = 1
        BLACK = 2

        trees_vertices = list()
        colors = [WHITE] * self.vertices_cnt
        time = 0
        d = [0] * self.vertices_cnt
        f = [0] * self.vertices_cnt

        for u in sorted(self.vertices, key=order_key, reverse=order_reverse):
            if colors[u] == WHITE:
                trees_vertices.append(dfs_visit(u))

        return trees_vertices, d, f

    def get_scc(self) -> list:
        """Creates SCC of the graph.

        Returns:
            list[DiGraph]: List of SCC.
        """
        _, _, f1 = self.dfs()
        graph_t = self.transpose()
        trees_vertices, _, _ = graph_t.dfs(order_key=lambda x: f1[x], order_reverse=True)
        return [self.get_induced_graph(tree_v) for tree_v in trees_vertices]
