#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#define LIMITE 10

typedef struct {
    int num_vertices;
    int adjacencia[LIMITE][LIMITE];
} GrafoMatriz;

void inicializar(GrafoMatriz *grafo, int num_vertices);
void inserir_aresta(GrafoMatriz *grafo, int u, int v);
void inserir_arco(GrafoMatriz *grafo, int u, int v);
void exibir_matriz(GrafoMatriz *grafo);

#endif