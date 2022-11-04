"""
algorithms.py
A module with algorithms for cycles enumeration.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 02.11.2022
"""

from queue import Queue
import networkx as nx
import numpy as np
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
                cycles.append(path)
            elif v > min_path:
                if v not in path:
                    q.put(path + [v])

    return cycles

def __get_cycles_book(input_graph: digraph.DiGraph) -> list():
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
                        temp_path = list(curr_path)
                        cycles.append([int(graph.vertex_cname[v]) for v in temp_path])
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
                    hj - using Hongbo Liu and Jiaxin Wang algorithm

    Returns:
        list: List of cycles.
    """
    if algo == "nx":
        return __get_cycles_nx(input_graph)
    elif algo == "hj":
        return __get_cycles_hj(input_graph)
    elif algo == "book":
        return __get_cycles_book(input_graph)
    else:
        raise ValueError("Unknown algorithm name.")
