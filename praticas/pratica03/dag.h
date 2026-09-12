#ifndef DAG_H
#define DAG_H

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

typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

void grafolista_inicializar(GrafoLista *g, int num_vertices);
void grafolista_inserir_arco(GrafoLista *g, int u, int v);
void grafolista_exibir(GrafoLista *g);
void grafolista_liberar(GrafoLista *g);

Fila *fila_criar(int capacidade);
void fila_destruir(Fila *f);
int fila_vazia(const Fila *f);
int fila_cheia(const Fila *f);
void fila_inserir(Fila *f, int valor);
int fila_remover(Fila *f);

Pilha *pilha_criar(int capacidade);
void pilha_destruir(Pilha *p);
int pilha_vazia(const Pilha *p);
int pilha_cheia(const Pilha *p);
void pilha_push(Pilha *p, int valor);
int pilha_pop(Pilha *p);

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);

int eh_dag(GrafoLista *g);

#endif
