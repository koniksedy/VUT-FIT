"""
enum_cycles.py
An executable python3 application for the enumeration
of cycles in directed graphs. Cycles are printed to the stdout.
Authors: Bc. Jan Bíl
         Bc. Michal Šedý
Last change: 15.11.2022
"""

from argparse import ArgumentParser, Namespace
from gal.parser import Parser
from gal.digraph import DiGraph
from gal import algorithms


def parse_arguments() -> Namespace:
    opt_parser = ArgumentParser()

    algo = opt_parser.add_mutually_exclusive_group(required=True)
    algo.add_argument("--nx", action="store_true",
                      help="Enumerates cycles using Networkx library.")
    algo.add_argument("--bf", action="store_true",
                      help="Enumerates cycles using brute-force algorithm.")
    algo.add_argument("--hj", action="store_true",
                      help="Enumerates cycles using Hongbo Liu's and Jiaxin Wang's algorithm.")
    algo.add_argument("--wein", action="store_true",
                      help="Enumerates cycles using Herbert Weinblatt's algorithm.")

    input_data = opt_parser.add_mutually_exclusive_group(required=True)
    input_data.add_argument("input", type=str, nargs="?",
                            help="An input file with a graph.")
    input_data.add_argument("-c", "--complete", type=int, metavar="N",
                            help="Enumerates cycles of a complete graph with N nodes.")
    input_data.add_argument("-m", "--multicycle", type=int, metavar=("N", "M"), nargs=2,
                            help="Enumerates cycles of a cyclic graph with N nodes and M additional random edges.")
    input_data.add_argument("-n", "--nested", type=int, metavar="N",
                            help="Enumerates cycles of a nested graph with N nodes.")

    return opt_parser.parse_args()

def load_graph(input_file: str|None, complete: int|None, multicycle: tuple|None, nested: int|None) -> DiGraph:
    if input_file is not None:
        return Parser.parse(input_file)
    elif complete is not None:
        return DiGraph.create_complete_graph(complete)
    elif multicycle is not None:
        return DiGraph.create_multicycle_graph(*multicycle)
    elif nested is not None:
        return DiGraph.create_nested_graph(nested)
    else:
        raise RuntimeError("The construction of a graph was not selected.")

def enumerate_cycles(graph: DiGraph, nx: bool, bf: bool, hj: bool, wein: bool) -> list:
    if nx:
        return algorithms.get_cycles(graph, "nx")
    elif bf:
        return algorithms.get_cycles(graph, "bf")
    elif hj:
        return algorithms.get_cycles(graph, "hj")
    elif wein:
        return algorithms.get_cycles(graph, "wein")
    else:
        raise RuntimeError("The enumeration algorithm was not selected.")

def translate_vertices(graph: DiGraph, vertices: list):
    return filter(lambda x: graph.vertex_cname[x], vertices)


def main():
    args = parse_arguments()
    graph = load_graph(args.input, args.complete, args.multicycle, args.nested)
    # Get cycles with an internal representation of vertices
    print("|V|:", len(graph.vertices))
    print("|E|:", sum([len(vs) for vs in graph.edges]))
    cycles_inter_repr = enumerate_cycles(graph, args.nx, args.bf, args.hj, args.wein)
    print("c:", len(cycles_inter_repr))
    # with open(f"graphs/{args.multicycle[1]}-{len(cycles_inter_repr)}.grpf", "w") as fd:
    #     print(graph, file=fd)
    exit(0)
    for cycle in cycles_inter_repr:
        vertex_names = map(lambda x: str(graph.vertex_cname[x]), cycle)
        print(" ".join(vertex_names))


if __name__ == "__main__":
    main()
