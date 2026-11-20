#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

void criar_grafo_lista(GrafoLista *grafo, int n){
    grafo->n = n;
    grafo->adj = (No **)malloc(n * sizeof(No *));
    for (int i = 0; i < n; i++) {
        grafo->adj[i] = NULL;
    }
}

void inserir_aresta_lista(GrafoLista *grafo, int u, int v){
    No *novo_no = (No *)malloc(sizeof(No));
    novo_no->destino = v;
    novo_no->prox = grafo->adj[u];
    grafo->adj[u] = novo_no;

    novo_no = (No *)malloc(sizeof(No));
    novo_no->destino = u;
    novo_no->prox = grafo->adj[v];
    grafo->adj[v] = novo_no;
}

void remover_aresta_lista(GrafoLista *grafo, int u, int v){
    No *atual = grafo->adj[u];
    No *anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == v) {
            if (anterior == NULL) grafo->adj[u] = atual->prox;
            else anterior->prox = atual->prox;
            free(atual);
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }

    atual = grafo->adj[v];
    anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == u) {
            if (anterior == NULL) grafo->adj[v] = atual->prox;
            else anterior->prox = atual->prox;
            free(atual);
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

int grau_lista(GrafoLista *grafo, int v){
    int grau = 0;
    No *atual = grafo->adj[v];
    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }
    return grau;
}

int sao_adjacentes_lista(GrafoLista *grafo, int u, int v){
    No *atual = grafo->adj[u];
    while (atual != NULL) {
        if (atual->destino == v) return 1;
        atual = atual->prox;
    }
    return 0;
}

void liberar_grafo_lista(GrafoLista *grafo){
    for (int i = 0; i < grafo->n; i++) {
        No *atual = grafo->adj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(grafo->adj);
}