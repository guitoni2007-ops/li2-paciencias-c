#ifndef JOGO_H
#define JOGO_H

#include "carta.h"
#include "baralho.h"

// Número de colunas no Simple Simon.
#define NUM_COLUNAS_SIMON 10

// Máximo de cartas por coluna no Simple Simon.
#define MAX_CARTAS_SIMON 52

// Estrutura que guarda o estado do Simple Simon.
typedef struct {
    Carta colunas[NUM_COLUNAS_SIMON][MAX_CARTAS_SIMON];
    int tam_colunas[NUM_COLUNAS_SIMON];
    int fundacoes_concluidas;
} JogoState;

// Inicializa o estado com baralho nas 10 colunas.
int simon_inicializar(JogoState *estado, unsigned int seed);

// Move qty cartas da coluna cg para cd.
int simon_mover(JogoState *estado, int cg, int qtd, int cd);

// Deteta sequencia inteira do alvo e move pra base das concluídas.
void simon_limpar_fundacoes(JogoState *estado);

// Avalia se o estado do jogo é de ganho.
int simon_jogo_ganho(const JogoState *estado);

#endif