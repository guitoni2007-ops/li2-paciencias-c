#include "baralho.h"
#include <stdlib.h>

// Gera um baralho novo com todas as 52 cartas em ordem (do Ás ao Rei para cada um dos quatro naipes)
void baralho_inicial(Carta deck[52]) {
    int i = 0;
    int n = 0;
    while (n <= ESPADAS) {
        int v = 1; // v começa em 1 para cada naipe 
        while (v <= 13) {
            deck[i].v = (unsigned char)v;
            deck[i].n = (Naipe)n;
            i++;
            v++;
        }
        n++;
    }
}

// Troca duas cartas de lugar no baralho usando os seus endereços de memória
static void trocar_cartas(Carta *a, Carta *b) {
    Carta tmp = *a;
    *a = *b;
    *b = tmp;
}

// Baralha as cartas através de 100 trocas aleatórias
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

// Tira sempre a primeira carta que vem a seguir num array de cartas e avança a variável topo
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