#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

GrafoListaPonderado *criar_grafo(int num_vertices) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    g->num_vertices = num_vertices;
    g->adj = malloc(num_vertices * sizeof(Aresta *));
    for (int i = 0; i < num_vertices; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

void inserir_arco(GrafoListaPonderado *g, int u, int v, int peso) {
    Aresta *nova = malloc(sizeof(Aresta));
    nova->destino = v;
    nova->peso = peso;
    nova->prox = g->adj[u];
    g->adj[u] = nova;
}

void exibir_grafo(GrafoListaPonderado *g) {
    for (int u = 0; u < g->num_vertices; u++) {
        printf("%d:", u);
        for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            printf(" -> %d(%d)", a->destino, a->peso);
        }
        printf("\n");
    }
}

void liberar_grafo(GrafoListaPonderado *g) {
    for (int u = 0; u < g->num_vertices; u++) {
        Aresta *a = g->adj[u];
        while (a != NULL) {
            Aresta *prox = a->prox;
            free(a);
            a = prox;
        }
    }
    free(g->adj);
    free(g);
}

static int relaxar_arestas(GrafoListaPonderado *g, int *dist, int *pred) {
    int alterou = 0;
    for (int u = 0; u < g->num_vertices; u++) {
        if (dist[u] == INFINITO) continue;
        for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            int v = a->destino;
            if (dist[u] + a->peso < dist[v]) {
                dist[v] = dist[u] + a->peso;
                pred[v] = u;
                alterou = 1;
            }
        }
    }
    return alterou;
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int n = g->num_vertices;

    for (int i = 0; i < n; i++) {
        dist[i] = INFINITO;
        pred[i] = SEM_PRED;
    }
    dist[origem] = 0;


    for (int i = 1; i <= n - 1; i++) {
        if (!relaxar_arestas(g, dist, pred)) break;
    }
    for (int u = 0; u < n; u++) {
        if (dist[u] == INFINITO) continue;
        for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            if (dist[u] + a->peso < dist[a->destino]) {
                return 0;
            }
        }
    }
    return 1;
}

int encontrar_vertice_ciclo(GrafoListaPonderado *g, int *dist, int *pred) {
    for (int u = 0; u < g->num_vertices; u++) {
        if (dist[u] == INFINITO) continue;
        for (Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            int v = a->destino;
            if (dist[u] + a->peso < dist[v]) {
                dist[v] = dist[u] + a->peso;
                pred[v] = u;
                return v;
            }
        }
    }
    return SEM_PRED;
}

void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int vertice, int *pred) {
    int n = g->num_vertices;

    int v = vertice;
    for (int i = 0; i < n; i++) {
        v = pred[v];
    }

    int *ciclo = malloc((n + 1) * sizeof(int));
    int tam = 0;
    int atual = v;
    do {
        ciclo[tam++] = atual;
        atual = pred[atual];
    } while (atual != v);
    ciclo[tam++] = v;

    printf("Ciclo negativo: ");
    for (int i = tam - 1; i >= 0; i--) {
        printf("%d", ciclo[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    free(ciclo);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (destino == origem) {
        printf("%d", origem);
        return;
    }
    if (pred[destino] == SEM_PRED) {
        printf("sem caminho");
        return;
    }
    reconstruir_caminho(origem, pred[destino], pred);
    printf(" -> %d", destino);
}

void exibir_distancias(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    printf("Vertice | Distancia | Caminho\n");
    for (int v = 0; v < g->num_vertices; v++) {
        printf("%7d | ", v);
        if (dist[v] == INFINITO) {
            printf("%9s | sem caminho\n", "INF");
            continue;
        }
        printf("%9d | ", dist[v]);
        reconstruir_caminho(origem, v, pred);
        printf("\n");
    }
}
