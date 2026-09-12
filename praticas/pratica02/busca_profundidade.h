#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "busca_largura.h"


typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

Pilha *pilha_criar(int capacidade);
void pilha_destruir(Pilha *p);
int pilha_vazia(const Pilha *p);
int pilha_cheia(const Pilha *p);
void pilha_push(Pilha *p, int valor);
int pilha_pop(Pilha *p);

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *pred,
                    int *tempo_entrada, int *tempo_saida, int *tempo);

void dfs_iterativa(GrafoLista *g, int origem, int *visitado, int *pred);

int eh_bipartido(GrafoLista *g);

int contar_componentes(GrafoLista *g);

int tem_ciclo(GrafoLista *g);

#endif
