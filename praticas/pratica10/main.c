#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

#define CUSTO_MIN 1
#define CUSTO_MAX 100
#define TEMPO_MIN 0.05   /* segundos minimos de medicao por algoritmo */

/* adaptadores com a mesma assinatura, para medir todos do mesmo jeito */
typedef int (*AlgoritmoTSP)(const TSP *, int *);

static int forca_bruta(const TSP *t, int *rota) { return tsp_forca_bruta(t, rota); }
static int vizinho(const TSP *t, int *rota) { return tsp_vizinho_mais_proximo(t, 0, rota); }
static int insercao(const TSP *t, int *rota) { return tsp_insercao(t, 0, rota); }

/* executa o algoritmo repetidas vezes ate somar TEMPO_MIN e devolve a media em ms */
static double medir_ms(AlgoritmoTSP alg, const TSP *t, int *rota, int *custo) {
    int repeticoes = 0;
    clock_t ini = clock(), fim;
    do {
        *custo = alg(t, rota);
        repeticoes++;
        fim = clock();
    } while ((double)(fim - ini) / CLOCKS_PER_SEC < TEMPO_MIN);
    return 1000.0 * (fim - ini) / CLOCKS_PER_SEC / repeticoes;
}

static void testar_grafo(const char *nome, const Grafo *g) {
    int *seq = malloc(g->n * sizeof(int));

    printf("--- %s ---\n", nome);
    if (encontrar_caminho_hamiltoniano_bt(g, seq)) {
        printf("Caminho hamiltoniano: ");
        imprimir_sequencia(seq, g->n);
        printf("\n");
    } else {
        printf("Caminho hamiltoniano: nao existe\n");
    }

    TSP *t = grafo_para_tsp(g);
    int custo = tsp_forca_bruta(t, seq);
    printf("Reducao para TSP: custo otimo %d (n = %d) -> ciclo hamiltoniano %s\n",
           custo, g->n, custo == g->n ? "EXISTE" : "nao existe");
    if (custo == g->n) {
        printf("Ciclo: ");
        imprimir_sequencia(seq, g->n);
        printf(" -> %d\n", seq[0]);
    }
    printf("\n");

    tsp_liberar(t);
    free(seq);
}

static void exemplos_hamiltoniano(void) {
    printf("=== Caminho / ciclo hamiltoniano ===\n");

    /* casa: quadrado 0-1-2-3 com telhado 4 ligado a 2 e 3 -> tem ciclo */
    Grafo *casa = grafo_criar(5);
    grafo_inserir_aresta(casa, 0, 1);
    grafo_inserir_aresta(casa, 1, 2);
    grafo_inserir_aresta(casa, 2, 3);
    grafo_inserir_aresta(casa, 3, 0);
    grafo_inserir_aresta(casa, 2, 4);
    grafo_inserir_aresta(casa, 3, 4);
    testar_grafo("Grafo casa (5 vertices)", casa);
    grafo_liberar(casa);

    /* caminho simples 0-1-2-3: tem caminho mas nao tem ciclo */
    Grafo *linha = grafo_criar(4);
    grafo_inserir_aresta(linha, 0, 1);
    grafo_inserir_aresta(linha, 1, 2);
    grafo_inserir_aresta(linha, 2, 3);
    testar_grafo("Caminho 0-1-2-3", linha);
    grafo_liberar(linha);

    /* estrela com centro 0: nem caminho nem ciclo hamiltoniano */
    Grafo *estrela = grafo_criar(4);
    grafo_inserir_aresta(estrela, 0, 1);
    grafo_inserir_aresta(estrela, 0, 2);
    grafo_inserir_aresta(estrela, 0, 3);
    testar_grafo("Estrela K1,3", estrela);
    grafo_liberar(estrela);
}

static void experimento(void) {
    int tamanhos[] = {5, 8, 10, 12};
    int qtd = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("=== Experimento TSP: custos aleatorios em [%d, %d] ===\n", CUSTO_MIN, CUSTO_MAX);
    printf("%3s | %6s %12s | %6s %10s %6s | %6s %10s %6s\n",
           "n", "otimo", "forca bruta", "VMP", "tempo", "razao", "ins", "tempo", "razao");
    printf("----+---------------------+--------------------------+--------------------------\n");

    for (int i = 0; i < qtd; i++) {
        int n = tamanhos[i];
        TSP *t = tsp_aleatorio(n, CUSTO_MIN, CUSTO_MAX);
        int *rota = malloc(n * sizeof(int));
        int c_fb, c_vmp, c_ins;

        double ms_fb = medir_ms(forca_bruta, t, rota, &c_fb);
        double ms_vmp = medir_ms(vizinho, t, rota, &c_vmp);
        double ms_ins = medir_ms(insercao, t, rota, &c_ins);

        printf("%3d | %6d %9.3f ms | %6d %7.4f ms %6.2f | %6d %7.4f ms %6.2f\n",
               n, c_fb, ms_fb,
               c_vmp, ms_vmp, (double)c_vmp / c_fb,
               c_ins, ms_ins, (double)c_ins / c_fb);

        free(rota);
        tsp_liberar(t);
    }

    printf("\nVMP = vizinho mais proximo | ins = insercao do mais proximo\n");
    printf("razao = custo da heuristica / custo otimo (1.00 = achou o otimo)\n");
}

int main(void) {
    srand(42);   /* semente fixa: experimento reprodutivel */
    exemplos_hamiltoniano();
    experimento();
    return 0;
}
