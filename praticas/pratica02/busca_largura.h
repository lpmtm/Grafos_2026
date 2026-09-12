#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

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
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

void grafolista_inicializar(GrafoLista *g, int num_vertices);
void grafolista_inserir_aresta(GrafoLista *g, int u, int v);
void grafolista_exibir(GrafoLista *g);
void grafolista_liberar(GrafoLista *g);

Fila *fila_criar(int capacidade);
void fila_destruir(Fila *f);
int fila_vazia(const Fila *f);
int fila_cheia(const Fila *f);
void fila_inserir(Fila *f, int valor);
int fila_remover(Fila *f);

 * O chamador deve alocar dist e pred com tamanho g->num_vertices.
 */
void bfs(GrafoLista *g, int origem, int *dist, int *pred);

#endif
