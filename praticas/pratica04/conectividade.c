#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"

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

int grafolista_contar_arestas(GrafoLista *g) {
    int total = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        for (NoAdj *aux = g->adjacencia[i]; aux != NULL; aux = aux->prox) {
            total++;
        }
    }
    return total / 2;
}

int grafolista_tem_aresta(GrafoLista *g, int u, int v) {
    for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
        if (aux->vertice == v) {
            return 1;
        }
    }
    return 0;
}

void dfs_articulacoes(GrafoLista *g, int u, int pai, int *visitado,
                       int *descoberta, int *low, int *tempo,
                       int *eh_articulacao) {
    visitado[u] = 1;
    descoberta[u] = *tempo;
    low[u] = *tempo;
    (*tempo)++;
    int filhos = 0;

    for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
        int v = aux->vertice;
        if (v == pai) {
            continue;
        }
        if (!visitado[v]) {
            filhos++;
            dfs_articulacoes(g, v, u, visitado, descoberta, low, tempo, eh_articulacao);
            if (low[v] < low[u]) {
                low[u] = low[v];
            }
            if (pai != -1 && low[v] >= descoberta[u]) {
                eh_articulacao[u] = 1;
            }
        } else if (descoberta[v] < low[u]) {
            low[u] = descoberta[v];
        }
    }

    if (pai == -1 && filhos > 1) {
        eh_articulacao[u] = 1;
    }
}

int *detectar_articulacoes(GrafoLista *g, int *quantidade) {
    int visitado[MAX_VERTICES] = {0};
    int descoberta[MAX_VERTICES];
    int low[MAX_VERTICES];
    int eh_articulacao[MAX_VERTICES] = {0};
    int tempo = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        if (!visitado[i]) {
            dfs_articulacoes(g, i, -1, visitado, descoberta, low, &tempo, eh_articulacao);
        }
    }

    int total = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        if (eh_articulacao[i]) {
            total++;
        }
    }

    int *resultado = malloc(sizeof(int) * (size_t) (total > 0 ? total : 1));
    if (resultado == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para resultado.\n");
        exit(EXIT_FAILURE);
    }

    int idx = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        if (eh_articulacao[i]) {
            resultado[idx++] = i;
        }
    }

    *quantidade = total;
    return resultado;
}

static void dfs_pontes(GrafoLista *g, int u, int pai, int *visitado,
                        int *descoberta, int *low, int *tempo,
                        Aresta *pontes, int *quantidade, int capacidade) {
    visitado[u] = 1;
    descoberta[u] = *tempo;
    low[u] = *tempo;
    (*tempo)++;

    for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
        int v = aux->vertice;
        if (v == pai) {
            continue;
        }
        if (!visitado[v]) {
            dfs_pontes(g, v, u, visitado, descoberta, low, tempo, pontes, quantidade, capacidade);
            if (low[v] < low[u]) {
                low[u] = low[v];
            }
            if (low[v] > descoberta[u] && *quantidade < capacidade) {
                pontes[*quantidade].u = u;
                pontes[*quantidade].v = v;
                (*quantidade)++;
            }
        } else if (descoberta[v] < low[u]) {
            low[u] = descoberta[v];
        }
    }
}

Aresta *detectar_pontes(GrafoLista *g, int *quantidade) {
    int visitado[MAX_VERTICES] = {0};
    int descoberta[MAX_VERTICES];
    int low[MAX_VERTICES];
    int tempo = 0;

    int capacidade = grafolista_contar_arestas(g);
    if (capacidade < 1) {
        capacidade = 1;
    }

    Aresta *pontes = malloc(sizeof(Aresta) * (size_t) capacidade);
    if (pontes == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para pontes.\n");
        exit(EXIT_FAILURE);
    }

    *quantidade = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        if (!visitado[i]) {
            dfs_pontes(g, i, -1, visitado, descoberta, low, &tempo, pontes, quantidade, capacidade);
        }
    }

    return pontes;
}
