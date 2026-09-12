#include <stdio.h>
#include <stdlib.h>
#include "busca_profundidade.h"


Pilha *pilha_criar(int capacidade) {
    Pilha *p = malloc(sizeof(Pilha));
    if (p == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para Pilha.\n");
        exit(EXIT_FAILURE);
    }
    p->dados = malloc(sizeof(int) * (size_t) capacidade);
    if (p->dados == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para dados da Pilha.\n");
        free(p);
        exit(EXIT_FAILURE);
    }
    p->capacidade = capacidade;
    p->topo = -1;
    return p;
}

void pilha_destruir(Pilha *p) {
    if (p != NULL) {
        free(p->dados);
        free(p);
    }
}

int pilha_vazia(const Pilha *p) {
    return p->topo == -1;
}

int pilha_cheia(const Pilha *p) {
    return p->topo == p->capacidade - 1;
}

void pilha_push(Pilha *p, int valor) {
    if (pilha_cheia(p)) {
        fprintf(stderr, "Erro: pilha cheia.\n");
        return;
    }
    p->topo++;
    p->dados[p->topo] = valor;
}

int pilha_pop(Pilha *p) {
    if (pilha_vazia(p)) {
        fprintf(stderr, "Erro: pilha vazia.\n");
        return -1;
    }
    int valor = p->dados[p->topo];
    p->topo--;
    return valor;
}

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *pred,
                    int *tempo_entrada, int *tempo_saida, int *tempo) {
    visitado[u] = 1;
    tempo_entrada[u] = *tempo;
    (*tempo)++;

    for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
        int v = aux->vertice;
        if (!visitado[v]) {
            pred[v] = u;
            dfs_recursiva(g, v, visitado, pred, tempo_entrada, tempo_saida, tempo);
        }
    }

    tempo_saida[u] = *tempo;
    (*tempo)++;
}

void dfs_iterativa(GrafoLista *g, int origem, int *visitado, int *pred) {
    Pilha *p = pilha_criar(g->num_vertices * g->num_vertices + 1);

    pilha_push(p, origem);

    while (!pilha_vazia(p)) {
        int u = pilha_pop(p);
        if (visitado[u]) {
            continue;
        }
        visitado[u] = 1;

        for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
            int v = aux->vertice;
            if (!visitado[v]) {
                pred[v] = u;
                pilha_push(p, v);
            }
        }
    }

    pilha_destruir(p);
}


int eh_bipartido(GrafoLista *g) {
    int cor[MAX_VERTICES];
    for (int i = 0; i < g->num_vertices; i++) {
        cor[i] = -1;
    }

    for (int origem = 0; origem < g->num_vertices; origem++) {
        if (cor[origem] != -1) {
            continue;
        }

        cor[origem] = 0;
        Fila *f = fila_criar(g->num_vertices);
        fila_inserir(f, origem);

        while (!fila_vazia(f)) {
            int u = fila_remover(f);
            for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
                int v = aux->vertice;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    fila_inserir(f, v);
                } else if (cor[v] == cor[u]) {
                    fila_destruir(f);
                    return 0;
                }
            }
        }

        fila_destruir(f);
    }

    return 1;
}

int contar_componentes(GrafoLista *g) {
    int visitado[MAX_VERTICES] = {0};
    int pred[MAX_VERTICES];
    int componentes = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        pred[i] = -1;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        if (!visitado[i]) {
            componentes++;
            dfs_iterativa(g, i, visitado, pred);
        }
    }

    return componentes;
}

static int tem_ciclo_aux(GrafoLista *g, int u, int *visitado, int pai) {
    visitado[u] = 1;

    for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
        int v = aux->vertice;
        if (!visitado[v]) {
            if (tem_ciclo_aux(g, v, visitado, u)) {
                return 1;
            }
        } else if (v != pai) {
            return 1;
        }
    }

    return 0;
}

int tem_ciclo(GrafoLista *g) {
    int visitado[MAX_VERTICES] = {0};

    for (int i = 0; i < g->num_vertices; i++) {
        if (!visitado[i]) {
            if (tem_ciclo_aux(g, i, visitado, -1)) {
                return 1;
            }
        }
    }

    return 0;
}
