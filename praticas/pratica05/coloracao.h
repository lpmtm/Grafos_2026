#ifndef COLORACAO_H
#define COLORACAO_H

#define MAX_VERTICES 100

typedef struct NoAdj {
    int vertice;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    NoAdj *adjacencia[MAX_VERTICES];
    int num_vertices;
} GrafoLista;

void grafolista_inicializar(GrafoLista *g, int num_vertices);
void grafolista_inserir_aresta(GrafoLista *g, int u, int v);
void grafolista_exibir(GrafoLista *g);
void grafolista_liberar(GrafoLista *g);
int grafolista_grau(GrafoLista *g, int u);

int *coloracao_gulosa(GrafoLista *g, int *num_cores);
int *coloracao_welsh_powell(GrafoLista *g, int *num_cores);
int eh_bipartido(GrafoLista *g);

#endif
