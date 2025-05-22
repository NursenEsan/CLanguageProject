#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent an edge in the graph
struct Edge {
    int src, dest;
    double weight;
};

// Structure to represent a subset for union-find
struct Subset {
    int parent;
    int rank;
};

// Function to find the parent of a node in the disjoint set
int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Function to perform union of two sets
void unionSets(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Kruskal's MST algorithm
void kruskalMST(struct Edge edges[], int numNodes, int numEdges, FILE* outputFile) {
    struct Edge mst[numNodes]; // Array to store the resulting MST
    struct Subset subsets[numNodes]; // Array to track subsets

    // Initialize subsets and result array
    for (int i = 0; i < numNodes; i++) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    // Sort edges in non-decreasing order of weights
    for (int i = 0; i < numEdges - 1; i++) {
        for (int j = 0; j < numEdges - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                struct Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    int mstIndex = 0;
    int edgeIndex = 0;

    // Construct MST
    while (mstIndex < numNodes - 1 && edgeIndex < numEdges) {
        struct Edge nextEdge = edges[edgeIndex++];

        int x = find(subsets, nextEdge.src);
        int y = find(subsets, nextEdge.dest);

        if (x != y) {
            mst[mstIndex++] = nextEdge;
            unionSets(subsets, x, y);
        }
    }

    // Print the MST edges and total weight to the output file
    fprintf(outputFile, "MST Edges: ");
    double totalWeight = 0.0;
    for (int i = 0; i < mstIndex; i++) {
        fprintf(outputFile, "(%d, %d), ", mst[i].src, mst[i].dest);
        totalWeight += mst[i].weight;
    }
    fprintf(outputFile, "\nTotal Weight: %.2f\n", totalWeight);
}

int main() {
    char* inputFiles[] = {
        "10_node_graph_data.txt",
        "100_node_graph_data.txt",
        "1000_node_graph_data.txt",
        "10000_node_graph_data.txt",
        "1000000_node_graph_data.txt"
    };

    int numInputFiles = sizeof(inputFiles) / sizeof(inputFiles[0]);

    for (int fileIndex = 0; fileIndex < numInputFiles; fileIndex++) {
        char* inputFile = inputFiles[fileIndex];
        FILE* inputFilePtr = fopen(inputFile, "r");
        if (inputFilePtr == NULL) {
            printf("Error opening input file: %s\n", inputFile);
            continue;
        }

        int numNodes, numEdges;
        fscanf(inputFilePtr, "%d", &numNodes);
        fscanf(inputFilePtr, "%d", &numEdges);

        struct Edge edges[numEdges];

        // Read the edges from the input file
        for (int i = 0; i < numEdges; i++) {
            fscanf(inputFilePtr, "%d %d %lf", &edges[i].src, &edges[i].dest, &edges[i].weight);
        }

        fclose(inputFilePtr);

        // Create the output file name based on the input file name
        char outputFileName[256];
        strcpy(outputFileName, inputFile);
        strcat(outputFileName, "_output.txt");

        // Open the output file
        FILE* outputFile = fopen(outputFileName, "w");
        if (outputFile == NULL) {
            printf("Error creating output file: %s\n", outputFileName);
            continue;
        }

        // Start measuring execution time
        clock_t startTime = clock();

        // Call the MST algorithm and pass the output file
        printf("Input File: %s\n", inputFile);
        kruskalMST(edges, numNodes, numEdges, outputFile);
        printf("Output File: %s\n", outputFileName);

        // Calculate execution time
        clock_t endTime = clock();
        double executionTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

        // Get memory consumption
        size_t memoryUsage = sizeof(edges) + sizeof(outputFileName);

        // Write execution time and memory consumption to the output file
        fprintf(outputFile, "\nExecution Time: %.4f seconds\n", executionTime);
        fprintf(outputFile, "Memory Consumption: %zu bytes\n", memoryUsage);

        fclose(outputFile);
        printf("\n");
    }

    return 0;
}
