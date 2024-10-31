#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "graph.h"
#include <bits/getopt_core.h>

// Helper function to count reachable vertices from a given vertex using DFS.

int countReachableVertices(Graph *graph, int v, bool visited[])
{
    visited[v] = true;
    int count = 1; // Count the current vertex

    Node *adjList = graph->array[v].head;
    while (adjList != NULL)
    {
        int connectedVertex = adjList->vertex;
        if (!visited[connectedVertex])
        {
            count += countReachableVertices(graph, connectedVertex, visited);
        }
        adjList = adjList->next;
    }
    return count;
}

// Checks if removing an edge will disconnect the graph.(remove the edge and check 
//if the number of reachable vertices befor and after is the seem)

bool isValidNextEdge(Graph *graph, int src, int dest)
{
    // Check if it's the only edge left from src
    if (graph->array[src].head->next == NULL)
    {
        return true;
    }

    // Count vertices reachable from src before removing the edge
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int beforeRemovalCount = countReachableVertices(graph, src, visited);
    free(visited);

    // Temporarily remove the edge
    removeEdge(graph, src, dest);

    // Count vertices reachable from src after removing the edge
    visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int afterRemovalCount = countReachableVertices(graph, src, visited);
    free(visited);

    // Re-add the edge to restore the original graph
    addEdge(graph, src, dest);

    // If the number of reachable vertices is the same before and after removal,
    // then removing this edge does not disconnect the graph
    return beforeRemovalCount == afterRemovalCount;
}


//  Finds and prints the Euler circuit using Fleury's Algorithm.
 
void printEulerCircuit(Graph *graph, int start)
{
    Node *adjList = graph->array[start].head;

    // Traverse all edges starting from vertex `start`
    while (adjList != NULL)
    {
        int nextVertex = adjList->vertex;

        // Check if this is a valid edge to remove (not a bridge unless necessary)
        if (isValidNextEdge(graph, start, nextVertex))
        {
            printf("%d -> ", start);
            removeEdge(graph, start, nextVertex); // Remove the edge
            printEulerCircuit(graph, nextVertex); // Recur with the next vertex
            return;
        }

        adjList = adjList->next;
    }

    // Print the last vertex (end of circuit)
    printf("%d\n", start);
}

/**
 * Generates a random graph with a specified number of edges.
 * Ensures no duplicate edges and no self-loops.
 */
void generateRandomGraph(Graph *graph, int numEdges, unsigned int seed)
{
    srand(seed); // Seed the random number generator
    int edgeCount = 0;

    // Continue adding edges until the desired number is reached
    while (edgeCount < numEdges)
    {
        int src = rand() % graph->numVertices;
        int dest = rand() % graph->numVertices;

        // Ensure no self-loops and no duplicate edges
        if (src != dest && !edgeExists(graph, src, dest))
        {
            addEdge(graph, src, dest);
            edgeCount++; // Only increment if the edge is successfully added
        }
    }
}

/**
 * Checks if all vertices with non-zero degree are connected.
 */
bool isConnected(Graph *graph)
{
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));

    int startVertex = -1;
    for (int i = 0; i < graph->numVertices; i++)
    {
        if (graph->degrees[i] > 0)
        {
            startVertex = i;
            break;
        }
    }

    if (startVertex == -1)
    {
        return true; // No edges in the graph
    }

    int connectedVertices = countReachableVertices(graph, startVertex, visited);
    free(visited);

    for (int i = 0; i < graph->numVertices; i++)
    {
        if (graph->degrees[i] > 0 && connectedVertices <= 1)
        {
            return false;
        }
    }
    return true;
}

/**
 * Determines if the graph has an Euler circuit.
 */
bool hasEulerCircuit(Graph *graph)
{
    if (!isConnected(graph))
    {
        return false;
    }

    for (int i = 0; i < graph->numVertices; i++)
    {
        if (graph->degrees[i] % 2 != 0)
        {
            return false;
        }
    }

    return true;
}

/**
 * Main function to parse command-line arguments, generate a random graph,
 * and determine if it has an Euler circuit.
 */
int main(int argc, char *argv[])
{
    int vertices = 0;
    int edges = 0;
    unsigned int seed = (unsigned int)time(NULL); // Default seed is current time

    int opt;
    while ((opt = getopt(argc, argv, "v:e:s:")) != -1)
    {
        switch (opt)
        {
        case 'v':
            vertices = atoi(optarg);
            break;
        case 'e':
            edges = atoi(optarg);
            break;
        case 's':
            seed = (unsigned int)atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s -v vertices -e edges [-s seed]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Check for valid input
    if (vertices <= 0 || edges <= 0)
    {
        fprintf(stderr, "Vertices and edges must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    // Create the graph and generate a random graph with the specified parameters
    Graph *graph = createGraph(vertices);
    // createManualGraph( graph) ;
    generateRandomGraph(graph, edges, seed);
    printGraph(graph);

    // Check if the generated graph has an Euler circuit
    if (hasEulerCircuit(graph))
    {
        printf("\nThe graph has an Euler circuit. The circuit is:\n");
        printEulerCircuit(graph, 0); // Start at any vertex with non-zero degree
    }
    else
    {
        printf("\nThe graph does not have an Euler circuit.\n");
    }
    freeGraph(graph);

    return 0;
}
