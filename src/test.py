import sys
from gal.parser import Parser


graph = Parser.parse(sys.argv[1])
print("Before")
print("-"*80)
print(graph)
print("#"*80)
graph.prune_single_scc()
for g in graph.get_scc():
    print("After")
    print("-"*80)
    print(g)
    print("#"*80)
