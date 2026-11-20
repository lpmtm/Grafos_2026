#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

static void executar(GrafoListaPonderado *g, int origem) {
    int *dist = malloc(g->num_vertices * sizeof(int));
    int *pred = malloc(g->num_vertices * sizeof(int));

    exibir_grafo(g);
    printf("\nBellman-Ford a partir do vertice %d:\n", origem);

    if (bellman_ford(g, origem, dist, pred)) {
        exibir_distancias(g, origem, dist, pred);
    } else {
        printf("Ciclo de custo negativo detectado!\n");
        int v = encontrar_vertice_ciclo(g, dist, pred);
        reconstruir_ciclo_negativo(g, v, pred);
    }

    free(dist);
    free(pred);
}

int main(void) {
    GrafoListaPonderado *g1 = criar_grafo(5);
    inserir_arco(g1, 0, 1, 6);
    inserir_arco(g1, 0, 2, 7);
    inserir_arco(g1, 1, 2, 8);
    inserir_arco(g1, 1, 3, 5);
    inserir_arco(g1, 1, 4, -4);
    inserir_arco(g1, 2, 3, -3);
    inserir_arco(g1, 2, 4, 9);
    inserir_arco(g1, 3, 1, -2);
    inserir_arco(g1, 4, 0, 2);
    inserir_arco(g1, 4, 3, 7);

    printf("=== Grafo 1 (sem ciclo negativo) ===\n");
    executar(g1, 0);
    liberar_grafo(g1);

    GrafoListaPonderado *g2 = criar_grafo(5);
    inserir_arco(g2, 0, 1, 4);
    inserir_arco(g2, 1, 2, 2);
    inserir_arco(g2, 2, 3, -6);
    inserir_arco(g2, 3, 1, 3);
    inserir_arco(g2, 3, 4, 5);

    printf("\n=== Grafo 2 (com ciclo negativo) ===\n");
    executar(g2, 0);
    liberar_grafo(g2);

    return 0;
}
