#ifndef DAG_H
#define DAG_H

#define MAX_VERTICES 100

/* ---------------------------------------------------------
 * Grafo dirigido representado por Lista de Adjacencia
 * --------------------------------------------------------- */

/* No da lista encadeada de adjacencia */
typedef struct NoAdj {
    int vertice;
    struct NoAdj *prox;
} NoAdj;

/* Grafo dirigido (digrafo) representado por lista de adjacencia */
typedef struct {
    NoAdj *adjacencia[MAX_VERTICES];
    int num_vertices;
} GrafoLista;

/* ---------------------------------------------------------
 * Fila (FIFO) - usada pelo algoritmo de Kahn
 * --------------------------------------------------------- */
typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

/* ---------------------------------------------------------
 * Pilha (LIFO) - usada para empilhar a saida da DFS
 * --------------------------------------------------------- */
typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

/* ---- Operacoes do grafo dirigido em lista de adjacencia ---- */
void grafolista_inicializar(GrafoLista *g, int num_vertices);
void grafolista_inserir_arco(GrafoLista *g, int u, int v);
void grafolista_exibir(GrafoLista *g);
void grafolista_liberar(GrafoLista *g);

/* ---- Operacoes da fila ---- */
Fila *fila_criar(int capacidade);
void fila_destruir(Fila *f);
int fila_vazia(const Fila *f);
int fila_cheia(const Fila *f);
void fila_inserir(Fila *f, int valor);
int fila_remover(Fila *f);

/* ---- Operacoes da pilha ---- */
Pilha *pilha_criar(int capacidade);
void pilha_destruir(Pilha *p);
int pilha_vazia(const Pilha *p);
int pilha_cheia(const Pilha *p);
void pilha_push(Pilha *p, int valor);
int pilha_pop(Pilha *p);

/* ---------------------------------------------------------
 * Ordenacao topologica
 * ---------------------------------------------------------
 * Ambas as funcoes retornam um vetor alocado dinamicamente
 * (o chamador deve dar free()) com a ordem topologica dos
 * vertices, e preenchem *tamanho com g->num_vertices.
 *
 * Caso o grafo NAO seja um DAG (isto e, exista pelo menos um
 * ciclo), ambas retornam NULL e *tamanho recebe 0.
 */

/* Algoritmo de Kahn: BFS com fila, usando grau de entrada. */
int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);

/* Variante via DFS: empilha o vertice na saida (pos-ordem)
 * e a ordem topologica e obtida desempilhando tudo. */
int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);

/* Retorna 1 se o grafo for um DAG (acíclico), 0 caso contrario. */
int eh_dag(GrafoLista *g);

#endif
