#include <stdio.h>
#include "busca_largura.h"
#include "busca_profundidade.h"

static void imprimir_bfs(GrafoLista *g, int origem) {
    int dist[MAX_VERTICES];
    int pred[MAX_VERTICES];

    bfs(g, origem, dist, pred);

    printf("\n=== BFS a partir do vertice %d ===\n", origem);
    for (int i = 0; i < g->num_vertices; i++) {
        printf("vertice %d | dist = %2d | pred = %2d\n", i, dist[i], pred[i]);
    }
}

static void imprimir_dfs(GrafoLista *g, int origem) {
    int visitado[MAX_VERTICES] = {0};
    int pred[MAX_VERTICES];
    int tempo_entrada[MAX_VERTICES];
    int tempo_saida[MAX_VERTICES];
    int tempo = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        pred[i] = -1;
    }

    printf("\n=== DFS recursiva a partir do vertice %d ===\n", origem);
    for (int i = 0; i < g->num_vertices; i++) {
        if (!visitado[i]) {
            dfs_recursiva(g, i, visitado, pred, tempo_entrada, tempo_saida, &tempo);
        }
    }

    for (int i = 0; i < g->num_vertices; i++) {
        printf("vertice %d | pred = %2d | entrada = %2d | saida = %2d\n",
               i, pred[i], tempo_entrada[i], tempo_saida[i]);
    }
}

int main(void) {
    GrafoLista grafo;

    /* Grafo nao orientado com dois componentes:
     *   componente 1: 0-1-2-3-4  (com um ciclo 0-1-2-0)
     *   componente 2: 5-6
     */
    grafolista_inicializar(&grafo, 7);
    grafolista_inserir_aresta(&grafo, 0, 1);
    grafolista_inserir_aresta(&grafo, 1, 2);
    grafolista_inserir_aresta(&grafo, 2, 0); /* fecha o ciclo 0-1-2 */
    grafolista_inserir_aresta(&grafo, 2, 3);
    grafolista_inserir_aresta(&grafo, 3, 4);
    grafolista_inserir_aresta(&grafo, 5, 6);

    printf("Lista de adjacencia do grafo:\n");
    grafolista_exibir(&grafo);

    imprimir_bfs(&grafo, 0);
    imprimir_dfs(&grafo, 0);

    printf("\n=== Aplicacoes ===\n");
    printf("Numero de componentes conexos: %d\n", contar_componentes(&grafo));
    printf("O grafo possui ciclo? %s\n", tem_ciclo(&grafo) ? "Sim" : "Nao");
    printf("O grafo e bipartido? %s\n", eh_bipartido(&grafo) ? "Sim" : "Nao");

    grafolista_liberar(&grafo);

    /* Segundo grafo: um caminho simples, sem ciclos e bipartido */
    GrafoLista caminho;
    grafolista_inicializar(&caminho, 5);
    grafolista_inserir_aresta(&caminho, 0, 1);
    grafolista_inserir_aresta(&caminho, 1, 2);
    grafolista_inserir_aresta(&caminho, 2, 3);
    grafolista_inserir_aresta(&caminho, 3, 4);

    printf("\n\nLista de adjacencia do caminho 0-1-2-3-4:\n");
    grafolista_exibir(&caminho);

    printf("\n=== Aplicacoes (caminho) ===\n");
    printf("Numero de componentes conexos: %d\n", contar_componentes(&caminho));
    printf("O grafo possui ciclo? %s\n", tem_ciclo(&caminho) ? "Sim" : "Nao");
    printf("O grafo e bipartido? %s\n", eh_bipartido(&caminho) ? "Sim" : "Nao");

    grafolista_liberar(&caminho);

    return 0;
}
