import sys
from gal.parser import Parser


graph = Parser.parse(sys.argv[1])
print("Before")
print("-"*80)
print(graph)
print("#"*80)
graph.prune_single_scc()
print("After")
print("-"*80)
print(graph)
print("#"*80)
