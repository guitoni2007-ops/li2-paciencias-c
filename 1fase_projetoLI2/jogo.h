#ifndef JOGO_H
#define JOGO_H

#include "carta.h"
#include "baralho.h"

#define NUM_COLUNAS 7
#define MAX_CARTAS_COLUNA 5
#define MAX_STOCK 17 

// Estrutura de dados que centraliza todo o estado atual de uma partida 
typedef struct {
    Carta colunas[NUM_COLUNAS][MAX_CARTAS_COLUNA]; 
    int tam_colunas[NUM_COLUNAS];                  
    Carta stock[MAX_STOCK];                        
    int tam_stock;                                 
    Carta descarte;                                
} JogoState;

// Declaração das funções principais de lógica de jogo 
int inicializar_jogo(JogoState *estado, unsigned int seed);
int jogar_carta_coluna(JogoState *estado, int indice_coluna);
int comprar_carta(JogoState *estado);
int jogo_ganho(const JogoState *estado);
int jogo_perdido(const JogoState *estado);

#endif