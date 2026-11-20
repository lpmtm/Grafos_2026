#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cobertura.h"

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

int grafo_num_arestas(const Grafo *g) {
    int m = 0;
    for (int u = 0; u < g->n; u++)
        for (int v = u + 1; v < g->n; v++)
            m += g->adj[u][v];
    return m;
}

Grafo *grafo_aleatorio(int n, double p) {
    Grafo *g = grafo_criar(n);
    for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++)
            if ((double)rand() / RAND_MAX < p)
                grafo_inserir_aresta(g, u, v);
    return g;
}

int eh_cobertura(const Grafo *g, const int *C) {
    for (int u = 0; u < g->n; u++)
        for (int v = u + 1; v < g->n; v++)
            if (g->adj[u][v] && !C[u] && !C[v])
                return 0;
    return 1;
}

typedef struct {
    const Grafo *g;
    int *atual;     
    int *melhor;    
    int melhor_tam;
} EstadoBT;

static int pode_excluir(const EstadoBT *e, int v) {
    for (int u = 0; u < v; u++)
        if (e->g->adj[v][u] && !e->atual[u])
            return 0;
    return 1;
}

static void bt(EstadoBT *e, int v, int tam) {
    if (tam >= e->melhor_tam) return;

    if (v == e->g->n) {
        e->melhor_tam = tam;
        memcpy(e->melhor, e->atual, e->g->n * sizeof(int));
        return;
    }

    if (pode_excluir(e, v)) {
        e->atual[v] = 0;
        bt(e, v + 1, tam);
    }
    e->atual[v] = 1;
    bt(e, v + 1, tam + 1);
    e->atual[v] = 0;
}

int cobertura_minima_bt(const Grafo *g, int *C) {
    EstadoBT e;
    e.g = g;
    e.atual = calloc(g->n, sizeof(int));
    e.melhor = C;

    for (int i = 0; i < g->n; i++) C[i] = 1;
    e.melhor_tam = g->n + 1;

    bt(&e, 0, 0);

    free(e.atual);
    return e.melhor_tam;
}

int cobertura_gulosa(const Grafo *g, int *C) {
    int n = g->n;
    int tam = 0;
    int **resto = malloc(n * sizeof(int *));
    int *grau = calloc(n, sizeof(int));
    for (int u = 0; u < n; u++) {
        C[u] = 0;
        resto[u] = malloc(n * sizeof(int));
        memcpy(resto[u], g->adj[u], n * sizeof(int));
        for (int v = 0; v < n; v++)
            grau[u] += resto[u][v];
    }

    for (;;) {
        int escolhido = -1;
        for (int u = 0; u < n; u++)
            if (grau[u] > 0 && (escolhido == -1 || grau[u] > grau[escolhido]))
                escolhido = u;
        if (escolhido == -1) break;  

        C[escolhido] = 1;
        tam++;

        for (int v = 0; v < n; v++) {
            if (resto[escolhido][v]) {
                resto[escolhido][v] = resto[v][escolhido] = 0;
                grau[v]--;
            }
        }
        grau[escolhido] = 0;
    }

    for (int u = 0; u < n; u++)
        free(resto[u]);
    free(resto);
    free(grau);
    return tam;
}

void imprimir_cobertura(const int *C, int n) {
    printf("{");
    int primeiro = 1;
    for (int v = 0; v < n; v++) {
        if (C[v]) {
            printf(primeiro ? "%d" : ", %d", v);
            primeiro = 0;
        }
    }
    printf("}");
}
