#include <stdio.h>
#include <stdlib.h>
#include "dag.h"


void grafolista_inicializar(GrafoLista *g, int num_vertices) {
    g->num_vertices = num_vertices;
    for (int i = 0; i < num_vertices; i++) {
        g->adjacencia[i] = NULL;
    }
}

void grafolista_inserir_arco(GrafoLista *g, int u, int v) {
    NoAdj *novo = malloc(sizeof(NoAdj));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para NoAdj.\n");
        exit(EXIT_FAILURE);
    }
    novo->vertice = v;
    novo->prox = g->adjacencia[u];
    g->adjacencia[u] = novo;
}

void grafolista_exibir(GrafoLista *g) {
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%d:", i);
        for (NoAdj *aux = g->adjacencia[i]; aux != NULL; aux = aux->prox) {
            printf(" -> %d", aux->vertice);
        }
        printf("\n");
    }
}

void grafolista_liberar(GrafoLista *g) {
    for (int i = 0; i < g->num_vertices; i++) {
        NoAdj *aux = g->adjacencia[i];
        while (aux != NULL) {
            NoAdj *tmp = aux;
            aux = aux->prox;
            free(tmp);
        }
        g->adjacencia[i] = NULL;
    }
}

// fila

Fila *fila_criar(int capacidade) {
    Fila *f = malloc(sizeof(Fila));
    if (f == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para Fila.\n");
        exit(EXIT_FAILURE);
    }
    f->dados = malloc(sizeof(int) * (size_t) capacidade);
    if (f->dados == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para dados da Fila.\n");
        free(f);
        exit(EXIT_FAILURE);
    }
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    return f;
}

void fila_destruir(Fila *f) {
    if (f != NULL) {
        free(f->dados);
        free(f);
    }
}

int fila_vazia(const Fila *f) {
    return f->tamanho == 0;
}

int fila_cheia(const Fila *f) {
    return f->tamanho == f->capacidade;
}

void fila_inserir(Fila *f, int valor) {
    if (fila_cheia(f)) {
        fprintf(stderr, "Erro: fila cheia.\n");
        return;
    }
    f->dados[f->fim] = valor;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

int fila_remover(Fila *f) {
    if (fila_vazia(f)) {
        fprintf(stderr, "Erro: fila vazia.\n");
        return -1;
    }
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

//Pilha

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


int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;
    int grau_entrada[MAX_VERTICES] = {0};

    for (int u = 0; u < n; u++) {
        for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
            grau_entrada[aux->vertice]++;
        }
    }

    Fila *f = fila_criar(n);
    for (int i = 0; i < n; i++) {
        if (grau_entrada[i] == 0) {
            fila_inserir(f, i);
        }
    }

    int *ordem = malloc(sizeof(int) * (size_t) n);
    if (ordem == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para ordem.\n");
        fila_destruir(f);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    while (!fila_vazia(f)) {
        int u = fila_remover(f);
        ordem[count++] = u;

        for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
            int v = aux->vertice;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0) {
                fila_inserir(f, v);
            }
        }
    }

    fila_destruir(f);

    if (count != n) {
        free(ordem);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = n;
    return ordem;
}

static void dfs_topologica_aux(GrafoLista *g, int u, int *cor, Pilha *pilha, int *ciclo) {
    cor[u] = 1;

    for (NoAdj *aux = g->adjacencia[u]; aux != NULL && !(*ciclo); aux = aux->prox) {
        int v = aux->vertice;
        if (cor[v] == 0) {
            dfs_topologica_aux(g, v, cor, pilha, ciclo);
        } else if (cor[v] == 1) {
            *ciclo = 1;
        }
    }

    if (!(*ciclo)) {
        cor[u] = 2;
        pilha_push(pilha, u);
    }
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;
    int cor[MAX_VERTICES] = {0};
    int ciclo = 0;

    Pilha *pilha = pilha_criar(n);

    for (int i = 0; i < n && !ciclo; i++) {
        if (cor[i] == 0) {
            dfs_topologica_aux(g, i, cor, pilha, &ciclo);
        }
    }

    if (ciclo) {
        pilha_destruir(pilha);
        *tamanho = 0;
        return NULL;
    }

    int *ordem = malloc(sizeof(int) * (size_t) n);
    if (ordem == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para ordem.\n");
        pilha_destruir(pilha);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        ordem[i] = pilha_pop(pilha);
    }

    pilha_destruir(pilha);
    *tamanho = n;
    return ordem;
}


int eh_dag(GrafoLista *g) {
    int tamanho;
    int *ordem = ordenacao_topologica_kahn(g, &tamanho);

    if (ordem == NULL) {
        return 0;
    }

    free(ordem);
    return 1;
}
