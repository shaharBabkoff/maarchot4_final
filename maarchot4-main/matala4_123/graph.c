#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node");
        exit(EXIT_FAILURE);
    }
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}


Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for graph.\n");
        exit(EXIT_FAILURE);
    }
    graph->numVertices = numVertices;
    graph->degrees = (int*)calloc(numVertices, sizeof(int));
    if (graph->degrees == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for degrees array.\n");
        exit(EXIT_FAILURE);
    }
    graph->array = (AdjList*)malloc(numVertices * sizeof(AdjList));
    if (graph->array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for adjacency list array.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numVertices; i++) {
        graph->array[i].head = NULL;
    }

    return graph;
}


bool edgeExists(Graph* graph, int src, int dest) {
    Node* temp = graph->array[src].head;
    while (temp != NULL) {
        if (temp->vertex == dest) {
            return true; // Edge exists
        }
        temp = temp->next;
    }
    return false; // Edge does not exist
}

void addEdge(Graph* graph, int src, int dest) {
    // Add edge from src to dest
    if (!edgeExists(graph, src, dest)) {
        Node* newNode = createNode(dest);
        newNode->next = graph->array[src].head;
        graph->array[src].head = newNode;

        // Add edge from dest to src (since the graph is undirected)
        newNode = createNode(src);
        newNode->next = graph->array[dest].head;
        graph->array[dest].head = newNode;

        // Update the degrees of both vertices
        graph->degrees[src]++;
        graph->degrees[dest]++;
    }
}


void printGraph(Graph* graph) {
    for (int v = 0; v < graph->numVertices; v++) {
        Node* temp = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (temp) {
            printf("-> %d", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

void removeEdge(Graph* graph, int src, int dest) {
    // Remove edge from src to dest
    Node* prev = NULL;
    Node* temp = graph->array[src].head;
    while (temp != NULL) {
        if (temp->vertex == dest) {
            if (prev != NULL) {
                prev->next = temp->next;
            } else {
                graph->array[src].head = temp->next;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    // Remove edge from dest to src (since graph is undirected)
    prev = NULL;
    temp = graph->array[dest].head;
    while (temp != NULL) {
        if (temp->vertex == src) {
            if (prev != NULL) {
                prev->next = temp->next;
            } else {
                graph->array[dest].head = temp->next;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    // Update the degrees of both vertices
    graph->degrees[src]--;
    graph->degrees[dest]--;
}
void createManualGraph(Graph* graph) {
    int numEdges;
    printf("Enter the number of edges: ");
    scanf("%d", &numEdges);

    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        printf("Enter edge %d (source destination): ", i + 1);
        scanf("%d %d", &src, &dest);

        // Ensure the vertices are within bounds
        if (src >= graph->numVertices || dest >= graph->numVertices || src < 0 || dest < 0) {
            printf("Invalid edge! Vertices must be between 0 and %d.\n", graph->numVertices - 1);
            i--; // Retry this edge
        } else {
            addEdge(graph, src, dest);
        }
    }

    printf("Graph creation complete.\n");
}
void freeGraph(Graph* graph) {
    if (graph == NULL) {
        return;
    }

    // Free the adjacency list nodes
    for (int i = 0; i < graph->numVertices; i++) {
        Node* temp = graph->array[i].head;
        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }

    // Free the array and degrees array
    free(graph->array);
    free(graph->degrees);

    // Free the graph structure
    free(graph);
}
