#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "tps.h"

/* ======================= Instancia ======================= */

TSP *tsp_criar(int n) {
    TSP *t = malloc(sizeof(TSP));
    t->n = n;
    t->custo = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        t->custo[i] = calloc(n, sizeof(int));
    return t;
}

void tsp_liberar(TSP *t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++)
        free(t->custo[i]);
    free(t->custo);
    free(t);
}

TSP *tsp_aleatorio(int n, int min, int max) {
    TSP *t = tsp_criar(n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            t->custo[i][j] = t->custo[j][i] = min + rand() % (max - min + 1);
    return t;
}

int tsp_custo_rota(const TSP *t, const int *rota) {
    int total = 0;
    for (int i = 0; i < t->n; i++)
        total += t->custo[rota[i]][rota[(i + 1) % t->n]];
    return total;
}

/* ===================== Forca bruta ===================== */

typedef struct {
    const TSP *t;
    int *perm;         /* perm[0] = origem fixa; perm[1..n-1] sao permutados */
    int *melhor;
    int melhor_custo;
} EstadoFB;

/* gera as permutacoes de perm[pos..n-1] por trocas; parcial = custo ate perm[pos-1] */
static void permutar(EstadoFB *e, int pos, int parcial) {
    int n = e->t->n;
    if (pos == n) {
        int total = parcial + e->t->custo[e->perm[n - 1]][e->perm[0]];
        if (total < e->melhor_custo) {
            e->melhor_custo = total;
            memcpy(e->melhor, e->perm, n * sizeof(int));
        }
        return;
    }
    for (int i = pos; i < n; i++) {
        int tmp = e->perm[pos]; e->perm[pos] = e->perm[i]; e->perm[i] = tmp;
        permutar(e, pos + 1, parcial + e->t->custo[e->perm[pos - 1]][e->perm[pos]]);
        tmp = e->perm[pos]; e->perm[pos] = e->perm[i]; e->perm[i] = tmp;
    }
}

int tsp_forca_bruta(const TSP *t, int *rota) {
    if (t->n == 1) { rota[0] = 0; return 0; }

    EstadoFB e;
    e.t = t;
    e.perm = malloc(t->n * sizeof(int));
    e.melhor = rota;
    e.melhor_custo = INT_MAX;
    for (int i = 0; i < t->n; i++) e.perm[i] = i;

    permutar(&e, 1, 0);   /* origem 0 fixa: (n-1)! permutacoes */

    free(e.perm);
    return e.melhor_custo;
}

/* ================= Vizinho mais proximo ================= */

int tsp_vizinho_mais_proximo(const TSP *t, int origem, int *rota) {
    int n = t->n;
    int *visitado = calloc(n, sizeof(int));

    rota[0] = origem;
    visitado[origem] = 1;
    for (int pos = 1; pos < n; pos++) {
        int atual = rota[pos - 1], prox = -1;
        for (int v = 0; v < n; v++)
            if (!visitado[v] && (prox == -1 || t->custo[atual][v] < t->custo[atual][prox]))
                prox = v;
        rota[pos] = prox;
        visitado[prox] = 1;
    }

    free(visitado);
    return tsp_custo_rota(t, rota);
}

/* ================ Insercao do mais proximo ================ */

int tsp_insercao(const TSP *t, int origem, int *rota) {
    int n = t->n;
    if (n == 1) { rota[0] = origem; return 0; }

    int *no_ciclo = calloc(n, sizeof(int));
    int *dist = malloc(n * sizeof(int));   /* menor custo de v ate o ciclo */
    int tam = 0;

    rota[tam++] = origem;
    no_ciclo[origem] = 1;
    for (int v = 0; v < n; v++)
        dist[v] = t->custo[origem][v];

    while (tam < n) {
        /* cidade fora do ciclo mais proxima de alguma cidade do ciclo */
        int k = -1;
        for (int v = 0; v < n; v++)
            if (!no_ciclo[v] && (k == -1 || dist[v] < dist[k]))
                k = v;

        /* aresta (rota[i], rota[i+1]) cuja troca por i -> k -> i+1 acrescenta menos */
        int melhor_pos = 0, melhor_acrescimo = INT_MAX;
        for (int i = 0; i < tam; i++) {
            int a = rota[i], b = rota[(i + 1) % tam];
            int acrescimo = t->custo[a][k] + t->custo[k][b] - (tam > 1 ? t->custo[a][b] : 0);
            if (acrescimo < melhor_acrescimo) {
                melhor_acrescimo = acrescimo;
                melhor_pos = i + 1;
            }
        }

        memmove(&rota[melhor_pos + 1], &rota[melhor_pos], (tam - melhor_pos) * sizeof(int));
        rota[melhor_pos] = k;
        tam++;
        no_ciclo[k] = 1;

        for (int v = 0; v < n; v++)
            if (t->custo[k][v] < dist[v])
                dist[v] = t->custo[k][v];
    }

    free(no_ciclo);
    free(dist);
    return tsp_custo_rota(t, rota);
}

/* ======================= Reducao ======================= */

TSP *grafo_para_tsp(const Grafo *g) {
    TSP *t = tsp_criar(g->n);
    for (int i = 0; i < g->n; i++)
        for (int j = 0; j < g->n; j++)
            if (i != j)
                t->custo[i][j] = g->adj[i][j] ? 1 : 2;
    return t;
}
