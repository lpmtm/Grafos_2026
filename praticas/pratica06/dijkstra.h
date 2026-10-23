#ifndef dijkstra_h
#define dijkstra_h

#include <limits.h>

#define MAX_VERTICES 100
#define INFINITO INT_MAX

typedef struct NoAdj {
    int vertice;
    int peso;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    NoAdj *adjacencia[MAX_VERTICES];
    int num_vertices;
} GrafoListaPonderado;

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao;
} MinHeap;

void grafo_ponderado_inicializar(GrafoListaPonderado *g, int num_vertices);
void grafo_ponderado_inserir_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void grafo_ponderado_exibir(GrafoListaPonderado *g);
void grafo_ponderado_liberar(GrafoListaPonderado *g);

MinHeap *criar_min_heap(int capacidade);
void destruir_min_heap(MinHeap *h);
int heap_vazio(const MinHeap *h);
void inserir_heap(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia);

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
int *reconstruir_caminho(int origem, int destino, int *pred);

#endif