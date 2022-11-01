import sys
from gal.parser import Parser
from gal.digraph import DiGraph
from gal.algorithms import get_cycles

graph = DiGraph.create_complete_graph(3)
print(sorted(get_cycles(graph, "nx")))
