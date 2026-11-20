#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

static void imprimir_coloracao(const char *rotulo, int *cor, int n, int num_cores) {
    printf("%s (usou %d cores): ", rotulo, num_cores);
    for (int i = 0; i < n; i++) {
        printf("v%d=%d ", i, cor[i]);
    }
    printf("\n");
}

static void testar_grafo(const char *nome, GrafoLista *g) {
    printf("=== %s ===\n", nome);
    grafolista_exibir(g);

    int num_cores_guloso;
    int *cor_guloso = coloracao_gulosa(g, &num_cores_guloso);
    imprimir_coloracao("Coloracao gulosa       ", cor_guloso, g->num_vertices, num_cores_guloso);
    free(cor_guloso);

    int num_cores_wp;
    int *cor_wp = coloracao_welsh_powell(g, &num_cores_wp);
    imprimir_coloracao("Coloracao Welsh-Powell ", cor_wp, g->num_vertices, num_cores_wp);
    free(cor_wp);

    printf("O grafo e bipartido? %s\n\n", eh_bipartido(g) ? "Sim" : "Nao");
}

int main(void) {
    GrafoLista k4;
    grafolista_inicializar(&k4, 4);
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            grafolista_inserir_aresta(&k4, i, j);
        }
    }
    testar_grafo("Grafo 1: K4 (completo com 4 vertices)", &k4);
    grafolista_liberar(&k4);

    GrafoLista c6;
    grafolista_inicializar(&c6, 6);
    grafolista_inserir_aresta(&c6, 0, 1);
    grafolista_inserir_aresta(&c6, 1, 2);
    grafolista_inserir_aresta(&c6, 2, 3);
    grafolista_inserir_aresta(&c6, 3, 4);
    grafolista_inserir_aresta(&c6, 4, 5);
    grafolista_inserir_aresta(&c6, 5, 0);
    testar_grafo("Grafo 2: C6 (ciclo par, bipartido)", &c6);
    grafolista_liberar(&c6);

    GrafoLista c5;
    grafolista_inicializar(&c5, 5);
    grafolista_inserir_aresta(&c5, 0, 1);
    grafolista_inserir_aresta(&c5, 1, 2);
    grafolista_inserir_aresta(&c5, 2, 3);
    grafolista_inserir_aresta(&c5, 3, 4);
    grafolista_inserir_aresta(&c5, 4, 0);
    testar_grafo("Grafo 3: C5 (ciclo impar, nao bipartido)", &c5);
    grafolista_liberar(&c5);

    GrafoLista petersen;
    grafolista_inicializar(&petersen, 10);
    grafolista_inserir_aresta(&petersen, 0, 1);
    grafolista_inserir_aresta(&petersen, 1, 2);
    grafolista_inserir_aresta(&petersen, 2, 3);
    grafolista_inserir_aresta(&petersen, 3, 4);
    grafolista_inserir_aresta(&petersen, 4, 0);
    grafolista_inserir_aresta(&petersen, 5, 7);
    grafolista_inserir_aresta(&petersen, 7, 9);
    grafolista_inserir_aresta(&petersen, 9, 6);
    grafolista_inserir_aresta(&petersen, 6, 8);
    grafolista_inserir_aresta(&petersen, 8, 5);
    grafolista_inserir_aresta(&petersen, 0, 5);
    grafolista_inserir_aresta(&petersen, 1, 6);
    grafolista_inserir_aresta(&petersen, 2, 7);
    grafolista_inserir_aresta(&petersen, 3, 8);
    grafolista_inserir_aresta(&petersen, 4, 9);
    testar_grafo("Grafo 4: Petersen (numero cromatico 3)", &petersen);
    grafolista_liberar(&petersen);

    return 0;
}
