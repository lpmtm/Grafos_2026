#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

#define INFINITO INT_MAX
#define SEM_PRED -1

typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta *prox;
} Aresta;

typedef struct {
    int num_vertices;
    Aresta **adj;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo(int num_vertices);
void inserir_arco(GrafoListaPonderado *g, int u, int v, int peso);
void exibir_grafo(GrafoListaPonderado *g);
void liberar_grafo(GrafoListaPonderado *g);

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
int encontrar_vertice_ciclo(GrafoListaPonderado *g, int *dist, int *pred);
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int vertice, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);
void exibir_distancias(GrafoListaPonderado *g, int origem, int *dist, int *pred);

#endif
