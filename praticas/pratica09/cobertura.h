#ifndef COBERTURA_H
#define COBERTURA_H

typedef struct {
    int n;
    int **adj;  
} Grafo;

Grafo *grafo_criar(int n);
void grafo_liberar(Grafo *g);
void grafo_inserir_aresta(Grafo *g, int u, int v);
int grafo_num_arestas(const Grafo *g);
Grafo *grafo_aleatorio(int n, double p);
int eh_cobertura(const Grafo *g, const int *C);
int cobertura_minima_bt(const Grafo *g, int *C);
int cobertura_gulosa(const Grafo *g, int *C);
void imprimir_cobertura(const int *C, int n);

#endif
