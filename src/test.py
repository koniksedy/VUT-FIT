import sys
from gal.parser import Parser
from gal.digraph import DiGraph
from gal.algorithms import get_cycles

graph = Parser.parse(sys.argv[1])
# graph.prune_single_scc()
# for g in graph.get_scc():
#     print(g)
#     print("#"*80)

# graph = DiGraph.create_multicycle_graph(2, 10000)
# print(graph)

print(sorted(get_cycles(graph, "nx")))
