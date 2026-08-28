#include <stdio.h>
#include <stdlib.h>
#include "grafo_matriz.h"

void criar_grafo_matriz(GrafoMatriz *grafo, int n){
    grafo->n = n;
    grafo->adj = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        grafo->adj[i] = (int *)calloc(n, sizeof(int)); // já inicia com 0
    }
}

void inserir_aresta_matriz(GrafoMatriz *grafo, int u, int v){
    grafo->adj[u][v] = 1;
    grafo->adj[v][u] = 1; // grafo não direcionado
}

void remover_aresta_matriz(GrafoMatriz *grafo, int u, int v){
    grafo->adj[u][v] = 0;
    grafo->adj[v][u] = 0;
}

int grau_matriz(GrafoMatriz *grafo, int v){
    int grau = 0;
    for (int i = 0; i < grafo->n; i++) {
        if (grafo->adj[v][i] == 1) {
            grau++;
        }
    }
    return grau;
}

int sao_adjacentes_matriz(GrafoMatriz *grafo, int u, int v){
    return grafo->adj[u][v] == 1;
}

void liberar_grafo_matriz(GrafoMatriz *grafo){
    for (int i = 0; i < grafo->n; i++) {
        free(grafo->adj[i]);
    }
    free(grafo->adj);
}