import sys
from gal.parser import Parser
from gal.algorithms import get_cycles

graph = Parser.parse(sys.argv[1])
print(get_cycles(graph, "hj"))
