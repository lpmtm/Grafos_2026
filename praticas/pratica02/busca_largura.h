#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#define MAX_VERTICES 100

/* ---------------------------------------------------------
 * Grafo representado por Lista de Adjacencia
 * --------------------------------------------------------- */

/* No da lista encadeada de adjacencia */
typedef struct NoAdj {
    int vertice;
    struct NoAdj *prox;
} NoAdj;

/* Grafo nao orientado representado por lista de adjacencia */
typedef struct {
    NoAdj *adjacencia[MAX_VERTICES];
    int num_vertices;
} GrafoLista;

/* ---------------------------------------------------------
 * Fila (FIFO) para BFS - implementada como array circular
 * --------------------------------------------------------- */
typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

/* ---- Operacoes do grafo em lista de adjacencia ---- */
void grafolista_inicializar(GrafoLista *g, int num_vertices);
void grafolista_inserir_aresta(GrafoLista *g, int u, int v);
void grafolista_exibir(GrafoLista *g);
void grafolista_liberar(GrafoLista *g);

/* ---- Operacoes da fila ---- */
Fila *fila_criar(int capacidade);
void fila_destruir(Fila *f);
int fila_vazia(const Fila *f);
int fila_cheia(const Fila *f);
void fila_inserir(Fila *f, int valor);
int fila_remover(Fila *f);

/* ---- Busca em Largura ----
 * dist[i] recebe a distancia (numero de arestas) de "origem" ate i,
 * ou -1 se o vertice i nao for alcancavel.
 * pred[i] recebe o predecessor de i na arvore de busca, ou -1.
 * O chamador deve alocar dist e pred com tamanho g->num_vertices.
 */
void bfs(GrafoLista *g, int origem, int *dist, int *pred);

#endif
