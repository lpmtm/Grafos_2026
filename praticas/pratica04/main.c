#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"
#include "planaridade.h"

static void imprimir_articulacoes(GrafoLista *g) {
    int quantidade;
    int *articulacoes = detectar_articulacoes(g, &quantidade);

    printf("Vertices de corte (articulacoes): ");
    if (quantidade == 0) {
        printf("nenhum");
    } else {
        for (int i = 0; i < quantidade; i++) {
            printf("%d ", articulacoes[i]);
        }
    }
    printf("\n");

    free(articulacoes);
}

static void imprimir_pontes(GrafoLista *g) {
    int quantidade;
    Aresta *pontes = detectar_pontes(g, &quantidade);

    printf("Pontes (arestas de corte): ");
    if (quantidade == 0) {
        printf("nenhuma");
    } else {
        for (int i = 0; i < quantidade; i++) {
            printf("(%d,%d) ", pontes[i].u, pontes[i].v);
        }
    }
    printf("\n");

    free(pontes);
}

static void imprimir_planaridade(GrafoLista *g) {
    printf("Numero de vertices: %d | Numero de arestas: %d\n",
           g->num_vertices, grafolista_contar_arestas(g));
    printf("Passa no teste de Euler (m <= 3n-6)? %s\n",
           eh_planar_euler(g) ? "Sim" : "Nao");
    printf("Contem K5? %s\n", contem_k5(g) ? "Sim" : "Nao");
    printf("Contem K3,3? %s\n", contem_k33(g) ? "Sim" : "Nao");
    printf("Heuristica de Kuratowski conclui que e planar? %s\n",
           eh_planar_kuratowski(g) ? "Sim" : "Nao");
}

int main(void) {
    GrafoLista g1;
    grafolista_inicializar(&g1, 6);
    grafolista_inserir_aresta(&g1, 0, 1);
    grafolista_inserir_aresta(&g1, 1, 2);
    grafolista_inserir_aresta(&g1, 2, 0);
    grafolista_inserir_aresta(&g1, 2, 3);
    grafolista_inserir_aresta(&g1, 3, 4);
    grafolista_inserir_aresta(&g1, 4, 5);
    grafolista_inserir_aresta(&g1, 5, 3);

    printf("=== Grafo 1: dois triangulos ligados por uma ponte ===\n");
    grafolista_exibir(&g1);
    imprimir_articulacoes(&g1);
    imprimir_pontes(&g1);
    printf("\n");
    grafolista_liberar(&g1);

    GrafoLista g2;
    grafolista_inicializar(&g2, 5);
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            grafolista_inserir_aresta(&g2, i, j);
        }
    }

    printf("=== Grafo 2: K5 (completo com 5 vertices) ===\n");
    imprimir_planaridade(&g2);
    printf("\n");
    grafolista_liberar(&g2);

    GrafoLista g3;
    grafolista_inicializar(&g3, 6);
    grafolista_inserir_aresta(&g3, 0, 3);
    grafolista_inserir_aresta(&g3, 0, 4);
    grafolista_inserir_aresta(&g3, 0, 5);
    grafolista_inserir_aresta(&g3, 1, 3);
    grafolista_inserir_aresta(&g3, 1, 4);
    grafolista_inserir_aresta(&g3, 1, 5);
    grafolista_inserir_aresta(&g3, 2, 3);
    grafolista_inserir_aresta(&g3, 2, 4);
    grafolista_inserir_aresta(&g3, 2, 5);

    printf("=== Grafo 3: K3,3 (completo bipartido) ===\n");
    imprimir_planaridade(&g3);
    printf("\n");
    grafolista_liberar(&g3);

    GrafoLista g4;
    grafolista_inicializar(&g4, 4);
    grafolista_inserir_aresta(&g4, 0, 1);
    grafolista_inserir_aresta(&g4, 1, 2);
    grafolista_inserir_aresta(&g4, 2, 3);
    grafolista_inserir_aresta(&g4, 3, 0);
    grafolista_inserir_aresta(&g4, 0, 2);

    printf("=== Grafo 4: quadrado com uma diagonal (planar) ===\n");
    imprimir_planaridade(&g4);
    grafolista_liberar(&g4);

    return 0;
}
