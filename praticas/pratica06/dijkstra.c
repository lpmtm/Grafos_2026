#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

void grafo_ponderado_inicializar(GrafoListaPonderado *g, int num_vertices) {
    g->num_vertices = num_vertices;
    for (int i = 0; i < num_vertices; i++) {
        g->adjacencia[i] = NULL;
    }
}

static void inserir_no_inicio(GrafoListaPonderado *g, int origem, int destino, int peso) {
    NoAdj *novo = malloc(sizeof(NoAdj));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para NoAdj.\n");
        exit(EXIT_FAILURE);
    }
    novo->vertice = destino;
    novo->peso = peso;
    novo->prox = g->adjacencia[origem];
    g->adjacencia[origem] = novo;
}

void grafo_ponderado_inserir_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    if (peso < 0) {
        fprintf(stderr, "Erro: Dijkstra nao aceita pesos negativos.\n");
        return;
    }
    inserir_no_inicio(g, u, v, peso);
    inserir_no_inicio(g, v, u, peso);
}

void grafo_ponderado_exibir(GrafoListaPonderado *g) {
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%d:", i);
        for (NoAdj *aux = g->adjacencia[i]; aux != NULL; aux = aux->prox) {
            printf(" -> %d(%d)", aux->vertice, aux->peso);
        }
        printf("\n");
    }
}

void grafo_ponderado_liberar(GrafoListaPonderado *g) {
    for (int i = 0; i < g->num_vertices; i++) {
        NoAdj *aux = g->adjacencia[i];
        while (aux != NULL) {
            NoAdj *tmp = aux;
            aux = aux->prox;
            free(tmp);
        }
        g->adjacencia[i] = NULL;
    }
}

MinHeap *criar_min_heap(int capacidade) {
    MinHeap *h = malloc(sizeof(MinHeap));
    if (h == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para MinHeap.\n");
        exit(EXIT_FAILURE);
    }

    h->dados = malloc(sizeof(NoHeap) * (size_t) capacidade);
    h->posicao = malloc(sizeof(int) * (size_t) capacidade);
    if (h->dados == NULL || h->posicao == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para dados do MinHeap.\n");
        free(h->dados);
        free(h->posicao);
        free(h);
        exit(EXIT_FAILURE);
    }

    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int i = 0; i < capacidade; i++) {
        h->posicao[i] = -1;
    }
    return h;
}

void destruir_min_heap(MinHeap *h) {
    if (h != NULL) {
        free(h->dados);
        free(h->posicao);
        free(h);
    }
}

int heap_vazio(const MinHeap *h) {
    return h->tamanho == 0;
}

static void trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[i].distancia < h->dados[pai].distancia) {
            trocar(h, i, pai);
            i = pai;
        } else {
            break;
        }
    }
}

static void descer(MinHeap *h, int i) {
    for (;;) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        int menor = i;

        if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia) {
            menor = esq;
        }
        if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia) {
            menor = dir;
        }
        if (menor == i) {
            break;
        }
        trocar(h, i, menor);
        i = menor;
    }
}

void inserir_heap(MinHeap *h, int vertice, int distancia) {
    if (h->tamanho == h->capacidade) {
        fprintf(stderr, "Erro: heap cheio.\n");
        return;
    }
    h->dados[h->tamanho].vertice = vertice;
    h->dados[h->tamanho].distancia = distancia;
    h->posicao[vertice] = h->tamanho;
    h->tamanho++;
    subir(h, h->tamanho - 1);
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap vazio = {-1, INFINITO};
    if (heap_vazio(h)) {
        return vazio;
    }

    NoHeap raiz = h->dados[0];
    int ultimo = h->tamanho - 1;

    if (ultimo > 0) {
        trocar(h, 0, ultimo);
    }

    h->posicao[raiz.vertice] = -1;
    h->tamanho--;

    if (h->tamanho > 0) {
        descer(h, 0);
    }

    return raiz;
}

void diminuir_chave(MinHeap *h, int vertice, int nova_distancia) {
    int i = h->posicao[vertice];
    if (i == -1) {
        return;
    }
    if (nova_distancia < h->dados[i].distancia) {
        h->dados[i].distancia = nova_distancia;
        subir(h, i);
    }
}

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int n = g->num_vertices;

    for (int i = 0; i < n; i++) {
        dist[i] = INFINITO;
        pred[i] = -1;
    }
    dist[origem] = 0;

    MinHeap *heap = criar_min_heap(n);
    for (int i = 0; i < n; i++) {
        inserir_heap(heap, i, dist[i]);
    }

    while (!heap_vazio(heap)) {
        NoHeap atual = extrair_minimo(heap);
        if (atual.distancia == INFINITO) {
            break;
        }

        int u = atual.vertice;
        for (NoAdj *aux = g->adjacencia[u]; aux != NULL; aux = aux->prox) {
            int v = aux->vertice;
            if (heap->posicao[v] != -1 && dist[u] + aux->peso < dist[v]) {
                dist[v] = dist[u] + aux->peso;
                pred[v] = u;
                diminuir_chave(heap, v, dist[v]);
            }
        }
    }

    destruir_min_heap(heap);
}

int *reconstruir_caminho(int origem, int destino, int *pred) {
    int tamanho = 1;
    int atual = destino;

    while (atual != origem) {
        atual = pred[atual];
        if (atual == -1) {
            return NULL;
        }
        tamanho++;
    }

    int *caminho = malloc(sizeof(int) * (size_t) (tamanho + 1));
    if (caminho == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para caminho.\n");
        exit(EXIT_FAILURE);
    }

    caminho[tamanho] = -1;
    atual = destino;
    for (int i = tamanho - 1; i >= 0; i--) {
        caminho[i] = atual;
        atual = pred[atual];
    }

    return caminho;
}