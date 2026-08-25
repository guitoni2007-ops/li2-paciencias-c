#include "baralho.h"
#include <stdlib.h>

// Preenche o array de 52 cartas com valores de 1 a 13 para cada um dos 4 naipes 
void baralho_inicial(Carta deck[52]) {
    int i = 0;
    int n = 0;
    while (n <= ESPADAS) {
        int v = 1;               // v começa em 1 para cada naipe 
        while (v <= 13) {
            deck[i].v = (unsigned char)v;
            deck[i].n = (Naipe)n;
            i++;
            v++;
        }
        n++;
    }
}

// Troca a posição de duas cartas na memória usando um ponteiro temporário 
static void trocar_cartas(Carta *a, Carta *b) {
    Carta tmp = *a;
    *a = *b;
    *b = tmp;
}

// Mistura o baralho realizando 100 trocas aleatórias entre posições do array 
void baralho_embaralhado(Carta deck[52], unsigned int seed) {
    int k = 0, i = 0, j = 0;
    srand(seed);
    while (k < 100) { 
        i = rand() % 52;
        j = rand() % 52;
        trocar_cartas(&deck[i], &deck[j]);
        k++;
    }
}

// Retira uma carta da posição indicada pelo topo e incrementa o índice para a próxima 
int baralho_tira(Carta deck[52], int *topo, Carta *out) {
    int sucesso = 0;
    if (deck != NULL && topo != NULL && out != NULL) {
        if (*topo >= 0 && *topo < 52) {
            *out = deck[*topo];
            (*topo)++;
            sucesso = 1;
        }
    }
    return sucesso;
}