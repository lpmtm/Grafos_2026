#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "agm.h"

/* ======================= DSU ======================= */

DSU *dsu_criar(int n) {
    DSU *d = malloc(sizeof(DSU));
    d->n = n;
    d->pai = malloc(n * sizeof(int));
    d->rank = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        d->pai[i] = i;
        d->rank[i] = 0;
    }
    return d;
}

void dsu_liberar(DSU *d) {
    if (!d) return;
    free(d->pai);
    free(d->rank);
    free(d);
}

int find(DSU *d, int x) {
    if (d->pai[x] != x)
        d->pai[x] = find(d, d->pai[x]);   /* compressao de caminho */
    return d->pai[x];
}

int unir(DSU *d, int x, int y) {
    int rx = find(d, x);
    int ry = find(d, y);
    if (rx == ry) return 0;

    /* uniao por rank: a arvore mais baixa fica sob a mais alta */
    if (d->rank[rx] < d->rank[ry]) {
        d->pai[rx] = ry;
    } else if (d->rank[rx] > d->rank[ry]) {
        d->pai[ry] = rx;
    } else {
        d->pai[ry] = rx;
        d->rank[rx]++;
    }
    return 1;
}

/* ===================== Kruskal ===================== */

static int comparar_arestas(const void *a, const void *b) {
    const Aresta *x = a;
    const Aresta *y = b;
    return (x->peso > y->peso) - (x->peso < y->peso);
}

int kruskal(Aresta *arestas, int m, int n, Aresta *agm, int *qtd_agm) {
    /* ordenacao: O(E log E) */
    qsort(arestas, m, sizeof(Aresta), comparar_arestas);

    DSU *d = dsu_criar(n);
    int total = 0;
    *qtd_agm = 0;

    /* selecao: O(E * alfa(V)) */
    for (int i = 0; i < m && *qtd_agm < n - 1; i++) {
        if (unir(d, arestas[i].u, arestas[i].v)) {
            agm[(*qtd_agm)++] = arestas[i];
            total += arestas[i].peso;
        }
    }

    dsu_liberar(d);
    return total;
}

/* ======================= Prim ====================== */

/* lista de adjacencia simples */
typedef struct No {
    int destino, peso;
    struct No *prox;
} No;

/* heap binario minimo indexado por vertice (permite decrease-key) */
typedef struct {
    int *heap;   /* heap[i] = vertice na posicao i */
    int *pos;    /* pos[v]  = posicao de v no heap, ou -1 se fora */
    int *chave;  /* chave[v] = menor peso de aresta ligando v a arvore */
    int tam;
} Heap;

static void heap_trocar(Heap *h, int i, int j) {
    int vi = h->heap[i], vj = h->heap[j];
    h->heap[i] = vj;
    h->heap[j] = vi;
    h->pos[vj] = i;
    h->pos[vi] = j;
}

static void heap_subir(Heap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->chave[h->heap[p]] <= h->chave[h->heap[i]]) break;
        heap_trocar(h, i, p);
        i = p;
    }
}

static void heap_descer(Heap *h, int i) {
    for (;;) {
        int menor = i, e = 2 * i + 1, dir = 2 * i + 2;
        if (e < h->tam && h->chave[h->heap[e]] < h->chave[h->heap[menor]]) menor = e;
        if (dir < h->tam && h->chave[h->heap[dir]] < h->chave[h->heap[menor]]) menor = dir;
        if (menor == i) break;
        heap_trocar(h, i, menor);
        i = menor;
    }
}

static int heap_extrair_min(Heap *h) {
    int v = h->heap[0];
    h->tam--;
    if (h->tam > 0) {
        h->heap[0] = h->heap[h->tam];
        h->pos[h->heap[0]] = 0;
        heap_descer(h, 0);
    }
    h->pos[v] = -1;
    return v;
}

static void heap_diminuir_chave(Heap *h, int v, int nova) {
    h->chave[v] = nova;
    heap_subir(h, h->pos[v]);
}

int prim(const Aresta *arestas, int m, int n, int origem, Aresta *agm, int *qtd_agm) {
    /* monta lista de adjacencia (grafo nao orientado) */
    No **adj = calloc(n, sizeof(No *));
    No *nos = malloc(2 * (size_t)m * sizeof(No));
    for (int i = 0; i < m; i++) {
        No *a = &nos[2 * i], *b = &nos[2 * i + 1];
        a->destino = arestas[i].v; a->peso = arestas[i].peso; a->prox = adj[arestas[i].u]; adj[arestas[i].u] = a;
        b->destino = arestas[i].u; b->peso = arestas[i].peso; b->prox = adj[arestas[i].v]; adj[arestas[i].v] = b;
    }

    int *pai = malloc(n * sizeof(int));
    Heap h;
    h.heap = malloc(n * sizeof(int));
    h.pos = malloc(n * sizeof(int));
    h.chave = malloc(n * sizeof(int));
    h.tam = n;

    for (int v = 0; v < n; v++) {
        pai[v] = -1;
        h.chave[v] = INT_MAX;
        h.heap[v] = v;
        h.pos[v] = v;
    }
    h.chave[origem] = 0;
    heap_subir(&h, h.pos[origem]);

    int total = 0;
    *qtd_agm = 0;

    while (h.tam > 0) {
        int u = heap_extrair_min(&h);           /* O(log V) */
        if (h.chave[u] == INT_MAX) break;       /* restante inalcancavel */

        if (pai[u] != -1) {
            Aresta e = { pai[u], u, h.chave[u] };
            agm[(*qtd_agm)++] = e;
            total += h.chave[u];
        }

        /* atualizacao de chaves: cada aresta visitada no maximo 2x, O(log V) cada */
        for (No *p = adj[u]; p; p = p->prox) {
            int v = p->destino;
            if (h.pos[v] != -1 && p->peso < h.chave[v]) {
                pai[v] = u;
                heap_diminuir_chave(&h, v, p->peso);
            }
        }
    }

    free(h.heap);
    free(h.pos);
    free(h.chave);
    free(pai);
    free(nos);
    free(adj);
    return total;
}

void imprimir_arestas(const Aresta *a, int qtd) {
    for (int i = 0; i < qtd; i++)
        printf("  %d - %d  (peso %d)\n", a[i].u, a[i].v, a[i].peso);
}
