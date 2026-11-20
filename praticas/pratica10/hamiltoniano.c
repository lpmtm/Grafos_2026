#include <stdio.h>
#include <stdlib.h>
#include "hamiltoniano.h"

/* ======================= Grafo ======================= */

Grafo *grafo_criar(int n) {
    Grafo *g = malloc(sizeof(Grafo));
    g->n = n;
    g->adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        g->adj[i] = calloc(n, sizeof(int));
    return g;
}

void grafo_liberar(Grafo *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

void grafo_inserir_aresta(Grafo *g, int u, int v) {
    if (u == v) return;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

/* ================ Caminho hamiltoniano ================ */

/* tenta preencher caminho[pos..n-1]; caminho[pos-1] ja esta fixado */
static int bt(const Grafo *g, int *caminho, int *visitado, int pos) {
    if (pos == g->n) return 1;   /* todos os vertices foram usados */

    int ultimo = caminho[pos - 1];
    for (int v = 0; v < g->n; v++) {
        if (!visitado[v] && g->adj[ultimo][v]) {
            caminho[pos] = v;
            visitado[v] = 1;
            if (bt(g, caminho, visitado, pos + 1)) return 1;
            visitado[v] = 0;     /* desfaz a escolha */
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const Grafo *g, int *caminho) {
    if (g->n == 0) return 0;

    int *visitado = calloc(g->n, sizeof(int));
    int achou = 0;

    /* o caminho pode comecar em qualquer vertice */
    for (int ini = 0; ini < g->n && !achou; ini++) {
        caminho[0] = ini;
        visitado[ini] = 1;
        achou = bt(g, caminho, visitado, 1);
        visitado[ini] = 0;
    }

    free(visitado);
    return achou;
}

void imprimir_sequencia(const int *seq, int n) {
    for (int i = 0; i < n; i++)
        printf(i ? " -> %d" : "%d", seq[i]);
}
