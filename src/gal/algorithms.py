"""
algorithms.py
A module with algorithms for cycles enumeration.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 15.11.2022
"""

import numpy as np
import networkx as nx
from queue import Queue
from . import digraph


def __get_cycles_nx(graph: digraph.DiGraph) -> list():
    """Enumerates all cycles (elementary circuits) in a graph.
    It uses a nonrecursive, iterator/generator version of
    Johnson's algorithm [Finding all the elementary circuits of
    a directed graph. D. B. Johnson, SIAM Journal on Computing 4,
    no. 1, 77-84, 1975. https://doi.org/10.1137/0204007].
    The time complexity is O((n + m)*(c + 1)) for n nodes, m edges
    and c elementary circuits.

    Args:
        input_graph (DiGraph): DiGraph.

    Returns:
        list: List of cycles.
    """
    graph_nx = nx.DiGraph()

    graph_nx.add_nodes_from(graph.vertices)
    for u in graph.vertices:
        for v in graph.edges[u]:
            graph_nx.add_edge(u, v)

    return list(nx.simple_cycles(graph_nx))

def __get_cycles_hj(input_graph: digraph.DiGraph) -> list():
    """Enumerates all cycles (elementary circuits) in a graph.
    It uses Hongbo's and Jiaxin's algorithm with a queue
    [Hongbo Liu and Jiaxin Wang, "A new way to enumerate cycles
    in graph," Advanced Int'l Conference on Telecommunications
    and Int'l Conference on Internet and Web Applications and
    Services (AICT-ICIW'06), 2006, pp. 57-57, doi: 10.1109/AICT-ICIW.2006.22.]
    The time complexity is O((n + m)*(c + 1)) for n nodes, m edges
    and c elementary circuits.

    Args:
        input_graph (DiGraph): DiGraph.

    Returns:
        list: List of cycles.
    """
    # Simplify graph
    graph = digraph.DiGraph(input_graph)
    graph.prune_single_scc()

    cycles = list()
    # Init
    q = Queue()
    for v in graph.vertices:
        q.put([v])

    #Computation
    while not q.empty():
        # Get open path
        path = q.get()
        min_path = min(path)
        head, last = path[0], path[-1]
        # Extend existing open path.
        for v in graph.edges[last]:
            if v == head:
                # Report cycle.
                # Translace local vertex ids from scc to their ids from input_graph.
                cycles.append([input_graph.vertex_cname.inv[graph.vertex_cname[v]] for v in path])
            elif v > min_path:
                if v not in path:
                    q.put(path + [v])

    return cycles


def __get_cycle_wein(input_graph: digraph.DiGraph) -> list:
    """Enumerates all cycles (elementary circuits) in a graph.
    It uses Herbert Winblatt's algorithm with backtracking.
    [Herbert Weinblatt. 1972. A New Search Algorithm for Finding
    the Simple Cycles of a Finite Directed Graph. J. ACM 19,
    1 (Jan. 1972), 43–56. https://doi.org/10.1145/321679.321684]
    The time complexity is O((n + m)*(c + 1)) for n nodes, m edges
    and c elementary circuits.

    Args:
        input_graph (DiGraph): DiGraph.

    Returns:
        list: List of cycles.
    """
    def get_tail(L: list, node: int, is_cycle=False) -> list:
        """Returns a tail of given path with respect to the node.
        The time complexity is O(n) for n nodes.

        Args:
            L (list): Path
            node (int): graph node
            is_cycle (bool, optional): If True, the the first node of
                                       a path is ignored. Defaults to False.

        Returns:
            list: Path tail with respect to the vertex.
        """
        try:
            return L[1 + L.index(node, 1 if is_cycle else 0):]
        except ValueError:
            return list()

    def get_suitable_edge(TT: list) -> int|None:
        """Returns suitable successor of the path end.
        The time complexity is O(n) for n nodes.

        Args:
            TT (list): Path.

        Returns:
            int|None: Returns suitable successor of the path end.
        """
        u = TT[-1]
        for v in graph.edges[u]:
            if S_e[u][v] == 0:
                return v

        return None

    def concat(is_top_recursive_call: bool, TT: list, P: list) -> list:
        """Recursively creates new cycles as a combination of subsections
        of existing cycles with a examined path P.

        Args:
            is_top_recursive_call (bool): If True, than the function was called
                                          from examine. Otherwise the function
                                          was called recursively.
            TT (list): Examined path.
            P (list): Recursively constructed path.

        Returns:
            list: The function returns a list of cycles to add (when called
                  recursively; can contains duplicates), or a list of added cycles.
        """
        # Init
        cycle_tails = list()
        to_add = list()
        added = list()
        v = P[-1]

        # Test subsections of all cycles.
        for c in cycles:
            tail = get_tail(c, v, True)
            if not tail or tail in cycle_tails:
                continue
            cycle_tails.append(tail)

            if set(tail).intersection(P):
                continue

            if S_v[c[-1]] == 2:
                to_add.extend(concat(False, TT, P + tail))
                continue
            else:
                new_c = [c[-1]] + get_tail(TT, c[-1], False) + P + get_tail(c, P[-1], True)
                if is_top_recursive_call:
                    # The function was called from examine (non-recursively).
                    # Add a new cycle to the list of found cycles.
                    # if new_c not in added:
                    cycles.append(new_c)
                    added.append(new_c)
                else:
                    # The function was called recursively.
                    # Only marks the cycle for addition (to avoid duplicates).
                    to_add.append(new_c)

        if is_top_recursive_call:
            # Examine cycles founded by recursively called functions.
            # It only looks for duplicates within cycles created since last external call.
            for c in to_add:
                if c not in added:
                    assert c not in cycles, c
                    cycles.append(c)
                    added.append(c)
            return added

        return to_add

    def examine(TT: list, v: int) -> None:
        """Controls it the TT and edge to the node v can be path
        of some cycle.
        """
        if S_v[v] == 0:
            S_v[v] = 1
            TT.append(v)
            # go back to extend
        elif S_v[v] == 1:
            new_c = [v] + get_tail(TT, v) + [v]
            assert new_c not in cycles, new_c
            cycles.append(new_c)
            # go back to extend and if TT is empty, than go to the main cycle
        else:
            concat(True, TT, [v])
            # go back to extend and if TT is empty, than go to the main cycle

    def extend(TT: list) -> None:
        """Extends path TT with a suitable edge.
        """
        while TT:
            u = TT[-1]
            v = get_suitable_edge(TT)
            if v is None:
                S_v[u] = 2
                TT.pop(-1)
            else:
                S_e[u][v] = 2
                examine(TT, v)

    # Simplify graph
    graph = digraph.DiGraph(input_graph)
    graph.prune_single_scc()

    # Init
    cycles = list()
    TT = list()
    # S_e[u][v] == 0  ->  The edge (u, v) has never been on the TT.
    # S_e[u][v] == 2  ->  The edge (u, v) has been on the TT (and may still be).
    S_e = np.zeros((graph.vertices_cnt, graph.vertices_cnt), dtype=int)
    # S_v[v] == 0  ->  The node v has never been on the TT.
    # S_v[v] == 1  ->  The node v is now on the TT.
    # S_v[v] == 2  ->  The node v has been on the TT, but has been removed.
    S_v = [0] * graph.vertices_cnt

    # Calculation
    for v in graph.vertices:
        if S_v[v] == 0:
            S_v[v] = 1
            TT = [v]
            extend(TT)

    # Projects node ids from simplified graph to node ids of the original graph.
    # The time complexity is O(n * c), where n is a number of nodes and c is a number of cycles.
    return [[input_graph.vertex_cname.inv[graph.vertex_cname[v]] for v in c[:-1]] for c in cycles]

def _bet_cycles_bf(input_graph: digraph.DiGraph) -> list():
    """Enumerates all cycles (elementary circuits) in a graph.
    It uses algorithm proposed in a book by Narsingh Deo
    (page 287 Alg. 5), which is systematic and exhaustive
    search for directive circuits. [Deo, Narsingh. Graph
    Theory with Applications to Engineering and Computer
    Science. 1st ed. Dover Publications, 2017. Web. 14 Oct. 2022.]
    The time complexity is TODO for n nodes, m edges
    and c elementary circuits.

    Args:
        input_graph (DiGraph): DiGraph.

    Returns:
        list: List of cycles.
    """
    graph = digraph.DiGraph(input_graph)
    graph.prune_single_scc()
    cycles = list()
    for vertex in graph.vertices:
        #Find paths starting from each vertex
        curr_vertex = vertex
        H =  np.zeros((graph.vertices_cnt, graph.vertices_cnt), dtype=bool)
        curr_path = list()
        while curr_vertex >= 0:
            found = False
            for succ in graph.get_successors(curr_vertex):
                if H[curr_vertex][succ] == 0 and (succ not in curr_path or succ == vertex) and succ >= vertex:
                    #Take first available edge
                    curr_path.append(curr_vertex)
                    found = True
                    if succ == vertex:
                        #If cycle is found, add it to list and forbid this edge from further search
                        H[curr_path[-1]][succ] = True
                        # temp_path = list(curr_path)
                        # cycles.append([int(graph.vertex_cname[v]) for v in temp_path])
                        cycles.append([input_graph.vertex_cname.inv[graph.vertex_cname[v]] for v in curr_path])
                        curr_vertex = curr_path.pop(-1)
                    else:
                        curr_vertex = succ
                    break
            if not found:
                # If no available edge is found, go back one step, forbid previously taken edge and continue
                if not curr_path:
                    # If no available edge from first vertex, break the while loop
                    curr_vertex = -1
                    continue
                H[curr_path[-1]][curr_vertex] = True
                curr_vertex = curr_path.pop(-1)
    return cycles

def get_cycles(input_graph: digraph.DiGraph, algo="nx") -> list:
    """Enumerates all cycles (elementary circuits) in a graph.

    Args:
        input_graph (DiGraph): DiGraph.
        algo (str): A type of algorithm to be used.
                    nx - using networkx library
                    bf - using brute-force
                    hj - using Hongbo Liu and Jiaxin Wang algorithm
                    wein - using Weinblatt's algorithm

    Returns:
        list: List of cycles.
    """
    if algo == "nx":
        return __get_cycles_nx(input_graph)
    elif algo == "hj":
        return __get_cycles_hj(input_graph)
    elif algo == "wein":
        return __get_cycle_wein(input_graph)
    elif algo == "bf":
        return _bet_cycles_bf(input_graph)
    else:
        raise ValueError("Unknown algorithm name.")
