#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"


void grafolista_inicializar(GrafoLista *g, int num_vertices) {
    g->num_vertices = num_vertices;
    for (int i = 0; i < num_vertices; i++) {
        g->adjacencia[i] = NULL;
    }
}

static void inserir_no_inicio(GrafoLista *g, int origem, int destino) {
    NoAdj *novo = malloc(sizeof(NoAdj));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para NoAdj.\n");
        exit(EXIT_FAILURE);
    }
    novo->vertice = destino;
    novo->prox = g->adjacencia[origem];
    g->adjacencia[origem] = novo;
}

void grafolista_inserir_aresta(GrafoLista *g, int u, int v) {
    inserir_no_inicio(g, u, v);
    inserir_no_inicio(g, v, u);
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

void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    for (int i = 0; i < g->num_vertices; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }

    Fila *f = fila_criar(g->num_vertices);

    dist[origem] = 0;
    fila_inserir(f, origem);

    while (!fila_vazia(f)) {
        int u = fila_remover(f);
        for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
            int v = aux->vertice;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                fila_inserir(f, v);
            }
        }
    }

    fila_destruir(f);
}
