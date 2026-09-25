#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

#define MAX_VERTICES 100

typedef struct NoAdj {
    int vertice;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    NoAdj *adjacencia[MAX_VERTICES];
    int num_vertices;
} GrafoLista;

typedef struct {
    int u;
    int v;
} Aresta;

void grafolista_inicializar(GrafoLista *g, int num_vertices);
void grafolista_inserir_aresta(GrafoLista *g, int u, int v);
void grafolista_exibir(GrafoLista *g);
void grafolista_liberar(GrafoLista *g);
int grafolista_contar_arestas(GrafoLista *g);
int grafolista_tem_aresta(GrafoLista *g, int u, int v);

void dfs_articulacoes(GrafoLista *g, int u, int pai, int *visitado,
                       int *descoberta, int *low, int *tempo,
                       int *eh_articulacao);
int *detectar_articulacoes(GrafoLista *g, int *quantidade);

Aresta *detectar_pontes(GrafoLista *g, int *quantidade);

#endif
