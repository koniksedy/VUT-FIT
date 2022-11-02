"""
algorithms.py
A module with algorithms for cycles enumeration.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 02.11.2022
"""

import networkx as nx
import numpy as np
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

def __get_cycle_wein(input_graph: digraph.DiGraph):
    def concat(is_recursive: bool, TT: list, P: list) -> None:
        def get_tail(L: list, item) -> list:
            t = list()
            found = False
            for v in L:
                if found:
                    t.append(v)
                if v == item:
                    found = True
            return t

        cycle_tails = list()
        to_add = list()

        v = P[-1]
        for c in cycles:
            tail = get_tail(c, v)
            if not tail or tail in cycle_tails:
                continue
            cycle_tails.append(tail)

            if set(tail).intersection(P):
                continue

            if S_v[c[-1]] == 2:
                to_add.extend(concat(True, TT, P + tail))
                continue
            else:
                assert S_v[[c[-1]]] == 1
                new_c = [c[-1]] + get_tail(TT, c[-1]) + P + get_tail(c, P[-1])
                if is_recursive:
                    cycles.append(new_c)
                else:
                    to_add.append(new_c)

        if is_recursive:
            for c in set(to_add):
                cycles.append(c)

        return to_add

    def examine(TT: list, v: int) -> None:
        if S_v[v] == 0:
            S_v[v] = 1
            # go back to extend
        elif S_v[v] == 1:
            cycles.append(TT)
            TT.pop(-1)
            # go back to extend and if TT is empty, than go to main funciton
        else:
            concat(False, TT, [v])
            TT.pop(-1)
            # go back to extend and if TT is empty, than go to main funciton

    def get_suitable_vertex(TT: list):
        u = TT[-1]
        for v in graph.edges[u]:
            if S_e[u][v] == 0:
                return v

        return None

    def extend(TT: list) -> None:
        while TT:
            u = TT[-1]
            v = get_suitable_vertex(TT)
            if v is None:
                S_v[u] = 2
                TT.pop(-1)
            else:
                S_e[u][v] = 2
                TT.append(v)
                examine(TT, v)

    # Simplify graph
    graph = digraph.DiGraph(input_graph)
    graph.prune_single_scc()

    cycles = list()

    # Init
    S_v = [0] * graph.vertices_cnt
    S_e = np.zeros((graph.vertices_cnt, graph.vertices_cnt), dtype=int)
    TT = list()

    # Calculation
    TT = []
    for v in graph.vertices:
        if S_v[v] == 0:
            S_v[v] = 1
            TT = [v]
            extend(TT)


def get_cycles(input_graph: digraph.DiGraph, algo="nx") -> list:
    """Enumerates all cycles (elementary circuits) in a graph.

    Args:
        input_graph (DiGraph): DiGraph.
        algo (str): A type of algorithm to be used.
                    nx - using networkx library
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
    else:
        raise ValueError("Unknown algorithm name.")
