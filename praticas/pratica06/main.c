#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"

static char nome(int v) {
    return (char) ('A' + v);
}

static void imprimir_caminho(int origem, int destino, int *pred) {
    int *caminho = reconstruir_caminho(origem, destino, pred);

    printf("Caminho %c -> %c: ", nome(origem), nome(destino));
    if (caminho == NULL) {
        printf("nao existe caminho\n");
        return;
    }

    for (int i = 0; caminho[i] != -1; i++) {
        printf("%c", nome(caminho[i]));
        if (caminho[i + 1] != -1) {
            printf(" -> ");
        }
    }
    printf("\n");

    free(caminho);
}

static void executar(GrafoListaPonderado *g, int origem) {
    int dist[MAX_VERTICES];
    int pred[MAX_VERTICES];

    dijkstra(g, origem, dist, pred);

    printf("Distancias a partir de %c:\n", nome(origem));
    for (int i = 0; i < g->num_vertices; i++) {
        printf("  %c: ", nome(i));
        if (dist[i] == INFINITO) {
            printf("infinito");
        } else {
            printf("%d", dist[i]);
        }
        printf(" | predecessor: ");
        if (pred[i] == -1) {
            printf("-");
        } else {
            printf("%c", nome(pred[i]));
        }
        printf("\n");
    }

    printf("\n");
    for (int i = 0; i < g->num_vertices; i++) {
        if (i != origem) {
            imprimir_caminho(origem, i, pred);
        }
    }
}

int main(void) {
    GrafoListaPonderado g;
    grafo_ponderado_inicializar(&g, 6);

    grafo_ponderado_inserir_aresta(&g, 0, 1, 4);
    grafo_ponderado_inserir_aresta(&g, 0, 2, 2);
    grafo_ponderado_inserir_aresta(&g, 1, 2, 1);
    grafo_ponderado_inserir_aresta(&g, 1, 3, 5);
    grafo_ponderado_inserir_aresta(&g, 1, 4, 8);
    grafo_ponderado_inserir_aresta(&g, 2, 3, 4);
    grafo_ponderado_inserir_aresta(&g, 2, 4, 10);
    grafo_ponderado_inserir_aresta(&g, 3, 4, 2);
    grafo_ponderado_inserir_aresta(&g, 3, 5, 1);
    grafo_ponderado_inserir_aresta(&g, 4, 5, 3);

    printf("=== Grafo 1: grafo do enunciado (A a F) ===\n");
    grafo_ponderado_exibir(&g);
    printf("\n");
    executar(&g, 0);
    grafo_ponderado_liberar(&g);

    GrafoListaPonderado h;
    grafo_ponderado_inicializar(&h, 4);
    grafo_ponderado_inserir_aresta(&h, 0, 1, 3);
    grafo_ponderado_inserir_aresta(&h, 1, 2, 4);

    printf("\n=== Grafo 2: vertice D isolado ===\n");
    grafo_ponderado_exibir(&h);
    printf("\n");
    executar(&h, 0);
    grafo_ponderado_liberar(&h);

    return 0;
}