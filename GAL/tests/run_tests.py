"""
run_tests.py
An executable python3 application for automatic testing.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 24.11.2022
"""

import sys
import tqdm
import numpy as np
from itertools import product

# setting path
sys.path.append('../src')
from gal.digraph import DiGraph
from gal.algorithms import get_cycles


def normalize_cycle(cycle: list) -> list:
    if not cycle:
        return cycle
    min_idx = np.argmin(cycle)
    return cycle[min_idx:] + cycle[:min_idx]

def test_part_of_graph(graph: DiGraph, cycle: list, note: str):
    for v in cycle:
        if v not in graph.vertices:
            raise ValueError(f"Note: {note}. Node {v} is not part of a graph in a cycle {cycle}.")

def test_uniq(cycle: list, note: str):
    if len(cycle) != len(set(cycle)):
        raise ValueError(f"Note: {note}. Duplicit node in a cycle {cycle}.")

def test_cycle(graph: DiGraph, cycle: list, note: str):
    len_cycle = len(cycle)
    for u_idx in range(len_cycle):
        v_idx = (u_idx + 1) % len_cycle
        u = cycle[u_idx]
        v = cycle[v_idx]
        if v not in graph.edges[u]:
            raise ValueError(f"Note: {note}. Edge ({u}, {v}) does not exists in a graph for a cycle {cycle}.")

def test_all_algo(graph: DiGraph, note: str, verbose: bool) -> dict:
    results = {"bf": 0, "hj": 0, "wein": 0}
    nx_cycles = set(map(lambda x: tuple(normalize_cycle(x)), get_cycles(graph, "nx")))
    for algo in ["bf", "hj", "wein"]:
        try:
            cycles = list(map(lambda x: tuple(normalize_cycle(x)), get_cycles(graph, algo)))
            for cycle in cycles:
                test_uniq(cycle, f"{algo} - {note}")
                test_part_of_graph(graph, cycle, f"{algo} - {note}")
                test_cycle(graph, cycle, f"{algo} - {note}")
            cycles = set(cycles)
            if nx_cycles != cycles:
                raise ValueError(f"nx_cycles != cycles for {algo} - {note}")
            results[algo] += 1
        except ValueError as e:
            if verbose:
                print(e, file=sys.stderr)
    return [results["bf"], results["hj"], results["wein"]]


def run_complete_graphs(verbose = False) -> None:
    total = 0
    results = [0, 0, 0]
    for n in tqdm.tqdm(list(range(1, 6)) * 30,
                       desc="Testing complete graphs",
                       ascii=False,
                       file=sys.stdout):
        total += 1
        graph = DiGraph.create_complete_graph(n)
        results = list(map(lambda x, y: x + y, test_all_algo(graph, "complete", verbose), results))
    return total, results

def run_multicycle_graph(verbose = False) -> None:
    total = 0
    results = [0, 0, 0]
    for n, m in tqdm.tqdm(list(product(range(1, 20), range(0, 20))) * 30,
                          desc="Testing multi-cycle graphs",
                          ascii=False,
                          file=sys.stdout):
        total += 1
        graph = DiGraph.create_multicycle_graph(n, m)
        results = list(map(lambda x, y: x + y, test_all_algo(graph, "multi-cycle", verbose), results))
    return total, results

def run_nested_graph(verbose = False) -> None:
    total = 0
    results = [0, 0, 0]
    for n in tqdm.tqdm(list(range(1, 100)) * 30,
                       desc="Testing nested graphs",
                       ascii=False,
                       file=sys.stdout):
        total += 1
        graph = DiGraph.create_nested_graph(n)
        results = list(map(lambda x, y: x + y, test_all_algo(graph, "nested", verbose), results))
    return total, results


def main():
    # Parse program arguments
    verbose = "--verbose" in sys.argv
    if {"-h", "--help"}.intersection(sys.argv):
        print("usage: run_tests.py [-h] [--verbose]")
        exit(0)
    if not verbose and len(sys.argv) > 1:
        print("Too many program arguments.")
        sys.exit(1)
    if verbose and len(sys.argv) > 2:
        print("Too many program arguments.")
        sys.exit(1)

    # Testing
    c = run_complete_graphs(verbose)
    m = run_multicycle_graph(verbose)
    n = run_nested_graph(verbose)

    # Printing results
    for total, result, name in [(*c, "complete"), (*m, "multi-cycle"), (*n, "nested")]:
        print()
        print(f"Results for {name}")
        print("-" * 80)
        print(f"TOTAL:     {total}")
        print(f"BF PASS:   {result[0]}")
        print(f"HJ PASS:   {result[1]}")
        print(f"WEIN PASS: {result[2]}")


if __name__ == "__main__":
    main()
