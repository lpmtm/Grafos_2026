#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "busca_largura.h"

/* ---------------------------------------------------------
 * Pilha (LIFO) para DFS iterativa
 * --------------------------------------------------------- */
typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

/* ---- Operacoes da pilha ---- */
Pilha *pilha_criar(int capacidade);
void pilha_destruir(Pilha *p);
int pilha_vazia(const Pilha *p);
int pilha_cheia(const Pilha *p);
void pilha_push(Pilha *p, int valor);
int pilha_pop(Pilha *p);

/* ---- Busca em Profundidade ----
 * dfs_recursiva marca "visitado" e registra o tempo de
 * entrada e saida de cada vertice (util para classificar arestas).
 * "tempo" e um contador global compartilhado entre as chamadas
 * recursivas, deve iniciar em 0 e ser passado por referencia.
 */
void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *pred,
                    int *tempo_entrada, int *tempo_saida, int *tempo);

/* Versao iterativa da DFS, usando a Pilha explicita acima. */
void dfs_iterativa(GrafoLista *g, int origem, int *visitado, int *pred);

/* ---- Aplicacoes das buscas ---- */

/* Retorna 1 se o grafo for bipartido, 0 caso contrario.
 * Usa 2-coloracao via BFS, considerando todos os componentes. */
int eh_bipartido(GrafoLista *g);

/* Retorna a quantidade de componentes conexos do grafo. */
int contar_componentes(GrafoLista *g);

/* Retorna 1 se o grafo (nao orientado) possuir pelo menos um ciclo,
 * 0 caso contrario. Usa DFS controlando o vertice pai. */
int tem_ciclo(GrafoLista *g);

#endif
