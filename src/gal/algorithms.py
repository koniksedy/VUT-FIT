"""
algorithms.py
A module with algorithms for cycles enumeration.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 01.11.2022
"""

from queue import Queue
import networkx as nx
from . import digraph

def __get_cycles_nx(graph) -> list():
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
        for v in graph.vertices:
            if graph.edges_mx[u][v]:
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
    # Run for each strongly connected component separately.
    for scc in graph.get_scc():
        # Init
        q = Queue()
        for v in scc.vertices:
            q.put([v])

        #Computation
        while not q.empty():
            # Get open path
            path = q.get()
            min_path = min(path)
            head, last = path[0], path[-1]
            # Test if there is an edge from last vertex of a path to its head.
            if scc.edges_mx[last][head]:
                # Translace local vertex ids from scc to their ids from input_graph.
                cycles.append([input_graph.vertex_cname.inv[scc.vertex_cname[v]] for v in path])
            # Extend existing open path.
            for v in scc.get_successors(last):
                if v > min_path:
                    if v not in path:
                        q.put(path + [v])

    return cycles

def get_cycles(input_graph: digraph.DiGraph, algo="nx") -> list:
    """Enumerates all cycles (elementary circuits) in a graph.

    Args:
        input_graph (DiGraph): DiGraph.
        algo (str): A type of algorithm to be used.
                    nx - using networkx library
                    hj - using Hongbo Liu and Jiaxin Wang algorithm

    Returns:
        list: List of cycles.
    """
    if algo == "nx":
        return __get_cycles_nx(input_graph)
    elif algo == "hj":
        return __get_cycles_hj(input_graph)
    else:
        raise ValueError("Unknown algorithm name.")
