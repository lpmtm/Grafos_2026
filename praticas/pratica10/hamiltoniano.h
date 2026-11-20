#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

/* grafo nao orientado simples em matriz de adjacencia */
typedef struct {
    int n;
    int **adj;   /* adj[u][v] = 1 se existe a aresta {u, v} */
} Grafo;

Grafo *grafo_criar(int n);
void grafo_liberar(Grafo *g);
void grafo_inserir_aresta(Grafo *g, int u, int v);

/*
 * Procura um caminho hamiltoniano (passa por todos os vertices uma unica vez)
 * por backtracking: monta permutacoes dos vertices, estendendo o caminho
 * apenas por vizinhos ainda nao visitados e desfazendo a escolha quando trava.
 * Se encontrar, escreve a sequencia em caminho (n posicoes) e retorna 1;
 * caso contrario retorna 0. Pior caso O(n!).
 */
int encontrar_caminho_hamiltoniano_bt(const Grafo *g, int *caminho);

void imprimir_sequencia(const int *seq, int n);

#endif
