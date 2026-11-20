#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

/* instancia do Caixeiro Viajante (TSP) simetrico: grafo completo com custos */
typedef struct {
    int n;
    int **custo;   /* custo[i][j] = custo[j][i], custo[i][i] = 0 */
} TSP;

TSP *tsp_criar(int n);
void tsp_liberar(TSP *t);

/* instancia aleatoria com custos inteiros em [min, max] */
TSP *tsp_aleatorio(int n, int min, int max);

/* custo do ciclo rota[0] -> ... -> rota[n-1] -> rota[0] */
int tsp_custo_rota(const TSP *t, const int *rota);

/*
 * Forca bruta: fixa a origem 0 e testa todas as (n-1)! permutacoes
 * dos demais vertices. Escreve a rota otima em rota e retorna o custo.
 */
int tsp_forca_bruta(const TSP *t, int *rota);

/*
 * Vizinho mais proximo: parte de origem e vai sempre para a cidade
 * nao visitada mais barata. O(n^2).
 */
int tsp_vizinho_mais_proximo(const TSP *t, int origem, int *rota);

/*
 * Insercao do mais proximo: comeca com o ciclo {origem, cidade mais proxima}
 * e, a cada passo, escolhe a cidade fora do ciclo mais proxima de alguma
 * cidade do ciclo, inserindo-a na posicao de menor acrescimo de custo. O(n^3).
 */
int tsp_insercao(const TSP *t, int origem, int *rota);

/*
 * Reducao Ciclo Hamiltoniano -> TSP: custo 1 para arestas existentes no grafo
 * e 2 para as inexistentes. O grafo tem ciclo hamiltoniano se e somente se
 * o TSP otimo resultante custa exatamente n.
 */
TSP *grafo_para_tsp(const Grafo *g);

#endif
