import sys
from gal.parser import Parser
from gal.digraph import DiGraph
from gal.algorithms import get_cycles
from cProfile import run


def main():
    # graph = Parser.parse(sys.argv[1])
    # graph.prune_single_scc()
    # for g in graph.get_scc():
    #     print(g)
    #     print("#"*80)

    # graph = DiGraph.create_multicycle_graph(2, 0)
    graph = DiGraph.create_complete_graph(7)

    print(len(list(sorted(get_cycles(graph, "wein")))))


if __name__ == "__main__":
    run("main()", sort="cumtime")