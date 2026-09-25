#include "planaridade.h"

int eh_planar_euler(GrafoLista *g) {
    int n = g->num_vertices;
    int m = grafolista_contar_arestas(g);

    if (n < 3) {
        return 1;
    }

    return m <= 3 * n - 6;
}

int contem_k5(GrafoLista *g) {
    int n = g->num_vertices;
    if (n < 5) {
        return 0;
    }

    int v[5];
    for (v[0] = 0; v[0] <= n - 5; v[0]++) {
        for (v[1] = v[0] + 1; v[1] <= n - 4; v[1]++) {
            for (v[2] = v[1] + 1; v[2] <= n - 3; v[2]++) {
                for (v[3] = v[2] + 1; v[3] <= n - 2; v[3]++) {
                    for (v[4] = v[3] + 1; v[4] <= n - 1; v[4]++) {
                        int completo = 1;
                        for (int i = 0; i < 5 && completo; i++) {
                            for (int j = i + 1; j < 5 && completo; j++) {
                                if (!grafolista_tem_aresta(g, v[i], v[j])) {
                                    completo = 0;
                                }
                            }
                        }
                        if (completo) {
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

static int contar_bits(int mascara) {
    int total = 0;
    while (mascara > 0) {
        total += mascara & 1;
        mascara >>= 1;
    }
    return total;
}

static int testar_bipartido_completo(GrafoLista *g, int *v) {
    for (int mascara = 1; mascara < 63; mascara++) {
        if (contar_bits(mascara) != 3) {
            continue;
        }

        int ladoA[3];
        int ladoB[3];
        int idxA = 0;
        int idxB = 0;

        for (int i = 0; i < 6; i++) {
            if (mascara & (1 << i)) {
                ladoA[idxA++] = v[i];
            } else {
                ladoB[idxB++] = v[i];
            }
        }

        int completo = 1;
        for (int i = 0; i < 3 && completo; i++) {
            for (int j = 0; j < 3 && completo; j++) {
                if (!grafolista_tem_aresta(g, ladoA[i], ladoB[j])) {
                    completo = 0;
                }
            }
        }

        if (completo) {
            return 1;
        }
    }

    return 0;
}

int contem_k33(GrafoLista *g) {
    int n = g->num_vertices;
    if (n < 6) {
        return 0;
    }

    int v[6];
    for (v[0] = 0; v[0] <= n - 6; v[0]++) {
        for (v[1] = v[0] + 1; v[1] <= n - 5; v[1]++) {
            for (v[2] = v[1] + 1; v[2] <= n - 4; v[2]++) {
                for (v[3] = v[2] + 1; v[3] <= n - 3; v[3]++) {
                    for (v[4] = v[3] + 1; v[4] <= n - 2; v[4]++) {
                        for (v[5] = v[4] + 1; v[5] <= n - 1; v[5]++) {
                            if (testar_bipartido_completo(g, v)) {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int eh_planar_kuratowski(GrafoLista *g) {
    if (!eh_planar_euler(g)) {
        return 0;
    }

    if (g->num_vertices <= 10) {
        if (contem_k5(g) || contem_k33(g)) {
            return 0;
        }
    }

    return 1;
}
