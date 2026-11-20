#include <stdio.h>
#include <string.h>
#include "agm.h"

#define N 9
#define M 14

int main(void) {
    /* grafo classico de exemplo (CLRS), 9 vertices e 14 arestas; AGM = 37 */
    Aresta grafo[M] = {
        {0, 1, 4},  {0, 7, 8},  {1, 2, 8},  {1, 7, 11},
        {2, 3, 7},  {2, 8, 2},  {2, 5, 4},  {3, 4, 9},
        {3, 5, 14}, {4, 5, 10}, {5, 6, 2},  {6, 7, 1},
        {6, 8, 6},  {7, 8, 7}
    };

    Aresta agm[N - 1];
    int qtd;

    /* Kruskal ordena o vetor recebido, entao trabalha sobre uma copia */
    Aresta copia[M];
    memcpy(copia, grafo, sizeof(grafo));

    int peso_k = kruskal(copia, M, N, agm, &qtd);
    printf("=== Kruskal ===\n");
    imprimir_arestas(agm, qtd);
    printf("Peso total: %d (%d arestas)\n\n", peso_k, qtd);

    int peso_p = prim(grafo, M, N, 0, agm, &qtd);
    printf("=== Prim (origem 0) ===\n");
    imprimir_arestas(agm, qtd);
    printf("Peso total: %d (%d arestas)\n\n", peso_p, qtd);

    printf("Pesos %s\n", peso_k == peso_p ? "iguais: OK" : "DIFERENTES: ERRO");

    /* demonstracao da DSU */
    printf("\n=== DSU ===\n");
    DSU *d = dsu_criar(5);
    unir(d, 0, 1);
    unir(d, 2, 3);
    unir(d, 1, 3);
    printf("find(0)=%d find(3)=%d -> mesmo conjunto? %s\n",
           find(d, 0), find(d, 3), find(d, 0) == find(d, 3) ? "sim" : "nao");
    printf("find(4)=%d -> junto com 0? %s\n",
           find(d, 4), find(d, 4) == find(d, 0) ? "sim" : "nao");
    dsu_liberar(d);

    return 0;
}
