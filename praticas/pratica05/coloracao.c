#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

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

int grafolista_grau(GrafoLista *g, int u) {
    int grau = 0;
    for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
        grau++;
    }
    return grau;
}

static int menor_cor_disponivel(GrafoLista *g, int u, int *cor) {
    int usada[MAX_VERTICES] = {0};
    for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
        int v = aux->vertice;
        if (cor[v] != -1) {
            usada[cor[v]] = 1;
        }
    }
    int c = 0;
    while (usada[c]) {
        c++;
    }
    return c;
}

int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    int n = g->num_vertices;
    int *cor = malloc(sizeof(int) * (size_t) n);
    if (cor == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para cor.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        cor[i] = -1;
    }

    for (int u = 0; u < n; u++) {
        cor[u] = menor_cor_disponivel(g, u, cor);
    }

    int maior = -1;
    for (int i = 0; i < n; i++) {
        if (cor[i] > maior) {
            maior = cor[i];
        }
    }

    *num_cores = maior + 1;
    return cor;
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int n = g->num_vertices;
    int ordem[MAX_VERTICES];
    int grau[MAX_VERTICES];

    for (int i = 0; i < n; i++) {
        ordem[i] = i;
        grau[i] = grafolista_grau(g, i);
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (grau[ordem[j]] < grau[ordem[j + 1]]) {
                int tmp = ordem[j];
                ordem[j] = ordem[j + 1];
                ordem[j + 1] = tmp;
            }
        }
    }

    int *cor = malloc(sizeof(int) * (size_t) n);
    if (cor == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para cor.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        cor[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int u = ordem[i];
        cor[u] = menor_cor_disponivel(g, u, cor);
    }

    int maior = -1;
    for (int i = 0; i < n; i++) {
        if (cor[i] > maior) {
            maior = cor[i];
        }
    }

    *num_cores = maior + 1;
    return cor;
}

int eh_bipartido(GrafoLista *g) {
    int n = g->num_vertices;
    int cor[MAX_VERTICES];
    int fila[MAX_VERTICES];

    for (int i = 0; i < n; i++) {
        cor[i] = -1;
    }

    for (int origem = 0; origem < n; origem++) {
        if (cor[origem] != -1) {
            continue;
        }

        cor[origem] = 0;
        int inicio = 0;
        int fim = 0;
        fila[fim++] = origem;

        while (inicio < fim) {
            int u = fila[inicio++];
            for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
                int v = aux->vertice;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    fila[fim++] = v;
                } else if (cor[v] == cor[u]) {
                    return 0;
                }
            }
        }
    }

    return 1;
}
