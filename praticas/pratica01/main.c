#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main(void){
    printf("=== GRAFO EM MATRIZ DE ADJACENCIA ===\n");
    GrafoMatriz gm;
    criar_grafo_matriz(&gm, 5);
    inserir_aresta_matriz(&gm, 0, 1);
    inserir_aresta_matriz(&gm, 0, 2);
    inserir_aresta_matriz(&gm, 1, 3);

    printf("Grau do vertice 0: %d\n", grau_matriz(&gm, 0));
    printf("0 e 1 sao adjacentes? %s\n", sao_adjacentes_matriz(&gm, 0, 1) ? "sim" : "nao");

    remover_aresta_matriz(&gm, 0, 1);
    printf("Apos remover aresta (0,1): 0 e 1 sao adjacentes? %s\n",
           sao_adjacentes_matriz(&gm, 0, 1) ? "sim" : "nao");

    liberar_grafo_matriz(&gm);

    printf("\n=== GRAFO EM LISTA DE ADJACENCIA ===\n");
    GrafoLista gl;
    criar_grafo_lista(&gl, 5);
    inserir_aresta_lista(&gl, 0, 1);
    inserir_aresta_lista(&gl, 0, 2);
    inserir_aresta_lista(&gl, 1, 3);

    printf("Grau do vertice 0: %d\n", grau_lista(&gl, 0));
    printf("0 e 1 sao adjacentes? %s\n", sao_adjacentes_lista(&gl, 0, 1) ? "sim" : "nao");

    remover_aresta_lista(&gl, 0, 1);
    printf("Apos remover aresta (0,1): 0 e 1 sao adjacentes? %s\n",
           sao_adjacentes_lista(&gl, 0, 1) ? "sim" : "nao");

    liberar_grafo_lista(&gl);

    return 0;
}