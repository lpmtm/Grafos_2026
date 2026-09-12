#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

static void imprimir_ordem(const char *rotulo, int *ordem, int tamanho) {
    if (ordem == NULL) {
        printf("%s: NAO EXISTE (o grafo possui ciclo, nao e um DAG)\n", rotulo);
        return;
    }

    printf("%s: ", rotulo);
    for (int i = 0; i < tamanho; i++) {
        printf("%d", ordem[i]);
        if (i < tamanho - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

int main(void) {

    GrafoLista dag;
    grafolista_inicializar(&dag, 5);
    grafolista_inserir_arco(&dag, 0, 1);
    grafolista_inserir_arco(&dag, 0, 2);
    grafolista_inserir_arco(&dag, 1, 3);
    grafolista_inserir_arco(&dag, 2, 3);
    grafolista_inserir_arco(&dag, 3, 4);

    printf("=== Grafo 1 (DAG) ===\n");
    printf("Lista de adjacencia:\n");
    grafolista_exibir(&dag);

    printf("\nO grafo e um DAG? %s\n\n", eh_dag(&dag) ? "Sim" : "Nao");

    int tamanho_kahn;
    int *ordem_kahn = ordenacao_topologica_kahn(&dag, &tamanho_kahn);
    imprimir_ordem("Ordenacao topologica (Kahn)     ", ordem_kahn, tamanho_kahn);
    free(ordem_kahn);

    int tamanho_dfs;
    int *ordem_dfs = ordenacao_topologica_dfs(&dag, &tamanho_dfs);
    imprimir_ordem("Ordenacao topologica (DFS)      ", ordem_dfs, tamanho_dfs);
    free(ordem_dfs);

    grafolista_liberar(&dag);

    GrafoLista com_ciclo;
    grafolista_inicializar(&com_ciclo, 3);
    grafolista_inserir_arco(&com_ciclo, 0, 1);
    grafolista_inserir_arco(&com_ciclo, 1, 2);
    grafolista_inserir_arco(&com_ciclo, 2, 0);

    printf("\n=== Grafo 2 (com ciclo) ===\n");
    printf("Lista de adjacencia:\n");
    grafolista_exibir(&com_ciclo);

    printf("\nO grafo e um DAG? %s\n\n", eh_dag(&com_ciclo) ? "Sim" : "Nao");

    int tamanho_kahn2;
    int *ordem_kahn2 = ordenacao_topologica_kahn(&com_ciclo, &tamanho_kahn2);
    imprimir_ordem("Ordenacao topologica (Kahn)     ", ordem_kahn2, tamanho_kahn2);
    free(ordem_kahn2);

    int tamanho_dfs2;
    int *ordem_dfs2 = ordenacao_topologica_dfs(&com_ciclo, &tamanho_dfs2);
    imprimir_ordem("Ordenacao topologica (DFS)      ", ordem_dfs2, tamanho_dfs2);
    free(ordem_dfs2);

    grafolista_liberar(&com_ciclo);

    return 0;
}
