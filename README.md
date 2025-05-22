# CLanguageProject
# Kruskal's Algorithm – C Implementation

This project implements **Kruskal's Algorithm** in C to compute the **Minimum Spanning Tree (MST)** for weighted undirected graphs. The program processes multiple input files of various graph sizes, measures the execution time and memory usage, and outputs the MST results to corresponding output files.


## How It Works
For each graph input file:
- Reads the number of nodes and edges.
- Reads all edges with weights.
- Applies Kruskal's algorithm to find MST using union-find.
- Writes the MST edges, total weight, execution time, and memory usage to a separate output file.