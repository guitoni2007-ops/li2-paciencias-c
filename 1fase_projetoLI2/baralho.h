#ifndef BARALHO_H
#define BARALHO_H
#include "carta.h"

/* Protótipos das funções de manipulação do conjunto de 52 cartas */
void baralho_inicial(Carta deck[52]);
void baralho_embaralhado(Carta deck[52], unsigned int seed);
int baralho_tira(Carta deck[52], int *topo, Carta *out);

#endif



