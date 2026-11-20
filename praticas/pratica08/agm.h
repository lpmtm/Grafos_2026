#ifndef AGM_H
#define AGM_H

/* ---------- Union-Find (Disjoint Set Union) ---------- */
typedef struct {
    int *pai, *rank;
    int n;
} DSU;

DSU *dsu_criar(int n);
void dsu_liberar(DSU *d);
int find(DSU *d, int x);            /* com compressao de caminho */
int unir(DSU *d, int x, int y);     /* uniao por rank; retorna 1 se uniu, 0 se ja estavam juntos */

/* ---------- Arestas ---------- */
typedef struct {
    int u, v, peso;
} Aresta;

/*
 * Kruskal: O(E log E)
 *   arestas    -> vetor com as m arestas do grafo (sera ordenado)
 *   n          -> numero de vertices (0..n-1)
 *   agm        -> vetor de saida com capacidade >= n-1
 *   qtd_agm    -> numero de arestas colocadas em agm
 *   retorno    -> peso total da AGM (ou floresta geradora, se desconexo)
 */
int kruskal(Aresta *arestas, int m, int n, Aresta *agm, int *qtd_agm);

/*
 * Prim com heap binario minimo + decrease-key: O((V + E) log V)
 *   arestas    -> vetor com as m arestas do grafo nao orientado
 *   n          -> numero de vertices (0..n-1)
 *   origem     -> vertice inicial
 *   agm        -> vetor de saida com capacidade >= n-1
 *   qtd_agm    -> numero de arestas colocadas em agm
 *   retorno    -> peso total da AGM (componente da origem)
 */
int prim(const Aresta *arestas, int m, int n, int origem, Aresta *agm, int *qtd_agm);

void imprimir_arestas(const Aresta *a, int qtd);

#endif
