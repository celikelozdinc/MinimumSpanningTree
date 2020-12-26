
# INTENTION : IMPLEMENT KRUSKAL ALGORITHM
Implementation of a minimum spanning tree`(MST)` algorithm for a weighted undirected graph.

## Spanning Tree
A tree that reaches all the nodes of the graph. The cost for such a spanning tree is the sum of all the edge costs in the spanning tree's edge set. Over all possible spanning trees, the MST is the one with the least total cost.

## Input Data
The file format will be an integer that is the node size of the graph and the further values will be integer triples: `(i,j,cost)`. Sample data can be seen in the files `mst_data.in, mst_data_1.in` and so on.

# COMPILE && RUN

```bash
g++ Traversal.cpp -o build/traversal
./build/traversal
```
