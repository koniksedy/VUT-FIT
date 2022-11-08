"""
digraph.py
DiGraph module.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 02.11.2022
"""

import random
from copy import deepcopy
from bidict import bidict


class DiGraph:
    """
    Attributes:
        vertices (set[int]): A set of renamed (id of the vertex) graph states.
        vertices_cnt (int): Cont of graph vertices.
        vertex_cname (bidict): Two-way dictionary with id (key) of vertex and original name (value).
        edges (list[list[int]]): Adjacent list of list of successors
        in_degree (list[int]): Input degree of a state.
        out_degree (list[int]): Output degree of a state.
    """
    def __init__(self, graph=None) -> None:
        """DiGraph constructor.

        Args:
            graph (DiGraph, optional): If not None, then the graph will be created as a deep copy.
                                       Defaults to None.
        """
        if graph is None:
            self.vertices = set()
            self.vertices_cnt = 0
            self.vertex_cname = bidict()
            self.edges = list()
            self.in_degree = list()
            self.out_degree = list()
        else:
            self.vertices = set(graph.vertices)
            self.vertices_cnt = graph.vertices_cnt
            self.vertex_cname = bidict(graph.vertex_cname)
            self.edges = deepcopy(graph.edges)
            self.in_degree = list(graph.in_degree)
            self.out_degree = list(graph.out_degree)

    def __eq__(self, __o: object) -> bool:
        if not isinstance(self, __o):
            return False
        return self.edges == __o.edges

    def __len__(self) -> int:
        return self.vertices_cnt

    def __str__(self) -> str:
        out = ""
        for u in self.vertices:
            for v in self.edges[u]:
                out += f"{self.vertex_cname[u]} {self.vertex_cname[v]}\n"
        return out

    def __repr__(self) -> str:
        return str(self.edges)

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
        graph.edges = [list(graph.vertices) for _ in range(graph.vertices_cnt)]
        graph.in_degree = [graph.vertices_cnt] * graph.vertices_cnt
        graph.out_degree = [graph.vertices_cnt] * graph.vertices_cnt
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
            graph.edges[u].append(v)

        # Add additional edges
        for u in random.choices(list(graph.vertices), k=additional_edges_cnt):
            possible_v = graph.vertices.difference(graph.edges[u])
            if possible_v:
                v = random.sample(possible_v, k=1)[0]
                graph.edges[u].append(v)
                graph.out_degree[u] += 1
                graph.in_degree[v] += 1

        return graph

    def set_vertices(self, vertices: set) -> None:
        """Prepare graph for given vertices. The previous graph data will be deleted.

        Args:
            vertices (set): Vertices
        """
        self.vertices_cnt = len(vertices)
        self.vertices = set(vertices)
        self.vertex_cname = bidict({i: v for i, v in zip(self.vertices, vertices)})
        self.edges = [list() for _ in range(self.vertices_cnt)]
        self.in_degree = [0] * self.vertices_cnt
        self.out_degree = [0] * self.vertices_cnt

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
            self.edges.append([])
            self.in_degree.append(0)
            self.out_degree.append(0)

    def add_edge(self, src: int, dst: int) -> None:
        """Adds edge between two vertices of the graph.

        Args:
            src (int): Id of the source vertex of the edge.
            dst (int): Id of the destination vertex of the edge.

        Raises:
            IndexError: The vertex does not exist.
        """
        if dst not in self.edges[src]:
            self.edges[src].append(dst)
            self.in_degree[dst] += 1
            self.out_degree[src] += 1

    def transpose(self):
        """Creates transposed graph.
        The time complexity is O(n + m), where n is a number of graph vertices
        and v is a number of graph edges.

        Returns:
            DiGraph: Transposed graph.
        """
        graph = DiGraph()
        graph.vertices = self.vertices
        graph.vertices_cnt = self.vertices_cnt
        graph.vertex_cname = self.vertex_cname
        graph.in_degree = list(self.out_degree)
        graph.out_degree = list(self.in_degree)
        graph.edges = [list() for _ in range(graph.vertices_cnt)]
        for u in self.vertices:
            for v in self.edges[u]:
                graph.edges[v].append(u)
        return graph

    def transpose_inplace(self) -> None:
        """Transpose graph in-place.
        The time complexity is O(n + m), where n is a number of graph vertices
        and v is a number of graph edges.
        """
        new_edges = [list() for _ in range(self.vertices_cnt)]
        for u in self.vertices:
            for v in self.edges[u]:
                new_edges[v].append(u)
        self.in_degree, self.out_degree = self.out_degree, self.in_degree

    def prune_single_scc(self) -> None:
        """Removes vertices with 0 input degree or 0 output degree.
        """
        to_prune = set()
        # Prune by in-degree = 0
        for u in self.get_topological_sort():
            if self.in_degree[u] == 0:
                to_prune.add(u)
                for v in self.edges[u]:
                    self.in_degree[v] -= 1

        # Prune by out-degree = 0
        graph_t = self.transpose()
        for u in graph_t.get_topological_sort():
            if graph_t.in_degree[u] == 0:
                to_prune.add(u)
                for v in graph_t.edges[u]:
                    graph_t.in_degree[v] -= 1

        # Remove vertices
        graph_induced = self.get_induced_graph(self.vertices.difference(to_prune))
        self.vertices = graph_induced.vertices
        self.vertices_cnt = graph_induced.vertices_cnt
        self.vertex_cname = graph_induced.vertex_cname
        self.edges = graph_induced.edges
        self.in_degree = graph_induced.in_degree
        self.out_degree = graph_induced.out_degree

    def get_successors(self, vertex_id: int) -> list:
        """Returns successors of the vertex.

        Args:
            vertex_id (int): Vertex

        Returns:
            list: Successors of the vertex.
        """
        return self.edges[vertex_id]

    def get_induced_graph(self, vertices: set):
        """Creates induced graph based on the set of given vertices.
        The time complexity is O(n + m), where n is a number of graph vertices
        and v is a number of graph edges.

        Args:
            vertices (set): Vertices of the new graph.

        Returns:
            DiGraph: Induced graph.
        """
        graph = DiGraph()
        if not vertices:
            return graph

        graph.set_vertices(range(len(vertices)))

        # Rebase edges and cname
        new_ids = {v_id: new_id for v_id, new_id in zip(vertices, graph.vertices)}
        for u in vertices:
            graph.vertex_cname[new_ids[u]] = self.vertex_cname[u]
            for v in self.edges[u]:
                if v in vertices:
                    graph.edges[new_ids[u]].append(new_ids[v])
                    graph.in_degree[new_ids[v]] += 1
                    graph.out_degree[new_ids[u]] += 1

        return graph

    def dfs(self, vertices_order: list = None) -> tuple:
        """Performs DFS.
        The time complexity is O(n + m), where n is a number of graph vertices
        and v is a number of graph edges.

        Args:
            vertices_order (list, optional): If set, then vertices will be chosen from this list
                                             by theirs order. If None, then vertices are chosen
                                             randomly. Defaults to None.
        Returns:
            tuple[list, list, list]: The method returns the list of trees (a set of vertices) in the DFS
                                     forest, the list of d times (times of the beginning of a work on
                                     vertices), and the list of f times (times of the end of a work on vertices).
                                     Trees and the vertices are sorted by the array f in ascend order.
        """
        def dfs_visit(s) -> set:
            nonlocal time
            finished = list()
            colors[s] = GRAY
            time += 1
            d[s] = time
            for v in self.get_successors(s):
                if colors[v] == WHITE:
                    finished.extend(dfs_visit(v))
            finished.append(s)
            colors[s] = BLACK
            time += 1
            f[s] = time
            return finished

        WHITE = 0
        GRAY = 1
        BLACK = 2

        trees_vertices = list()
        colors = [WHITE] * self.vertices_cnt
        time = 0
        d = [0] * self.vertices_cnt
        f = [0] * self.vertices_cnt

        vertices = self.vertices if vertices_order is None else vertices_order
        for u in vertices:
            if colors[u] == WHITE:
                trees_vertices.append(dfs_visit(u))

        return trees_vertices, d, f

    def get_scc(self) -> list:
        """Creates SCC of the graph.
        The time complexity is O(n + m), where n is a number of graph vertices
        and v is a number of graph edges.

        Returns:
            list[DiGraph]: List of SCC.
        """
        forest, _, _ = self.dfs()
        vertices = [v for tree in forest for v in tree]
        vertices.reverse()
        graph_t = self.transpose()
        forest, _, _ = graph_t.dfs(vertices)
        return [self.get_induced_graph(tree) for tree in forest]

    def get_topological_sort(self) -> list:
        """Returns graph vertices in a topological order.
        The time complexity is O(n + m), where n is a number of graph vertices
        and v is a number of graph edges.

        Returns:
            list: Vertices in a topological order.
        """
        forest, _, _ = self.dfs()
        vertices = [v for tree in forest for v in tree]
        vertices.reverse()
        return vertices
