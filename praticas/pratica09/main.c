#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

#define DENSIDADE 0.4
#define TEMPO_MIN 0.05  

typedef int (*AlgoritmoCobertura)(const Grafo *, int *);

static double medir_ms(AlgoritmoCobertura alg, const Grafo *g, int *C, int *tam) {
    int repeticoes = 0;
    clock_t ini = clock(), fim;
    do {
        *tam = alg(g, C);
        repeticoes++;
        fim = clock();
    } while ((double)(fim - ini) / CLOCKS_PER_SEC < TEMPO_MIN);
    return 1000.0 * (fim - ini) / CLOCKS_PER_SEC / repeticoes;
}

static void exemplo_pequeno(void) {
    Grafo *g = grafo_criar(6);
    grafo_inserir_aresta(g, 0, 1);
    grafo_inserir_aresta(g, 1, 2);
    grafo_inserir_aresta(g, 0, 3);
    grafo_inserir_aresta(g, 1, 4);
    grafo_inserir_aresta(g, 2, 5);
    grafo_inserir_aresta(g, 3, 4);
    grafo_inserir_aresta(g, 4, 5);

    int C[6];
    printf("=== Exemplo: grade 2x3 ===\n");

    int t = cobertura_minima_bt(g, C);
    printf("Backtracking: tamanho %d ", t);
    imprimir_cobertura(C, g->n);
    printf(" | eh_cobertura = %d\n", eh_cobertura(g, C));

    t = cobertura_gulosa(g, C);
    printf("Gulosa:       tamanho %d ", t);
    imprimir_cobertura(C, g->n);
    printf(" | eh_cobertura = %d\n", eh_cobertura(g, C));

    int vazio[6] = {0};
    printf("Conjunto vazio: eh_cobertura = %d\n\n", eh_cobertura(g, vazio));

    grafo_liberar(g);
}

static void experimento(void) {
    int tamanhos[] = {5, 8, 10, 12, 15};
    int qtd = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("=== Experimento: G(n, p=%.1f) ===\n", DENSIDADE);
    printf("%4s %4s | %8s %12s | %8s %12s | %6s %6s\n",
           "n", "m", "|C| BT", "tempo BT", "|C| gul", "tempo gul", "razao", "valida");
    printf("----------+-----------------------+-----------------------+--------------\n");

    for (int i = 0; i < qtd; i++) {
        int n = tamanhos[i];
        Grafo *g = grafo_aleatorio(n, DENSIDADE);
        int *Cbt = malloc(n * sizeof(int));
        int *Cgul = malloc(n * sizeof(int));
        int tam_bt, tam_gul;

        double ms_bt = medir_ms(cobertura_minima_bt, g, Cbt, &tam_bt);
        double ms_gul = medir_ms(cobertura_gulosa, g, Cgul, &tam_gul);

        double razao = tam_bt > 0 ? (double)tam_gul / tam_bt : 1.0;
        int validas = eh_cobertura(g, Cbt) && eh_cobertura(g, Cgul);

        printf("%4d %4d | %8d %9.4f ms | %8d %9.4f ms | %6.2f %6s\n",
               n, grafo_num_arestas(g), tam_bt, ms_bt, tam_gul, ms_gul,
               razao, validas ? "sim" : "NAO");

        free(Cbt);
        free(Cgul);
        grafo_liberar(g);
    }

    printf("\nrazao = |C gulosa| / |C otima| (1.00 = heuristica achou o otimo)\n");
}

int main(void) {
    srand(42);   
    exemplo_pequeno();
    experimento();
    return 0;
}
