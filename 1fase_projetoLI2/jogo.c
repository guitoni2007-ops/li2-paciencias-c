#include "jogo.h"
#include <stddef.h> 

// Verifica se a diferença de valor entre as cartas é de apenas uma unidade (maior ou menor) 
static int pode_jogar_carta(Carta topo, Carta jogada) {
    int diff = topo.v - jogada.v;
    int valido = 0;
    if (diff == 1 || diff == -1) { 
        valido = 1;
    }
    return valido;
}

// Preenche as 7 colunas do jogo com 5 cartas cada, retiradas do baralho misturado 
static int distribuir_colunas(JogoState *est, Carta deck[52], int *t) {
    int c = 0, l = 0, suc = 1;
    while (c < NUM_COLUNAS && suc == 1) {
        l = 0;
        while (l < MAX_CARTAS_COLUNA && suc == 1) {
            suc = baralho_tira(deck, t, &est->colunas[c][l]);
            l++;
        }
        est->tam_colunas[c] = MAX_CARTAS_COLUNA;
        c++;
    }
    return suc;
}

// Define a primeira carta de descarte e coloca as restantes no monte de reserva (stock) 
static int distribuir_resto(JogoState *est, Carta deck[52], int *t) {
    int suc = 0;
    if (est != NULL) {
        suc = baralho_tira(deck, t, &est->descarte);
        est->tam_stock = 0;
        while (*t < 52 && suc == 1) {
            suc = baralho_tira(deck, t, &est->stock[est->tam_stock]);
            est->tam_stock++;
        }
    }
    return suc;
}

// Prepara o estado inicial: cria baralho, mistura e distribui as cartas pela mesa 
int inicializar_jogo(JogoState *estado, unsigned int seed) {
    Carta deck[52];
    int topo = 0, suc = 0;
    if (estado != NULL) {
        baralho_inicial(deck);
        baralho_embaralhado(deck, seed);
        suc = distribuir_colunas(estado, deck, &topo);
        if (suc == 1) {
            suc = distribuir_resto(estado, deck, &topo);
        }
    }
    return suc;
}

// Tenta mover a carta do topo de uma coluna para o descarte se a regra de valor for cumprida 
int jogar_carta_coluna(JogoState *est, int c) {
    int movido = 0;
    if (est != NULL && c >= 0 && c < NUM_COLUNAS && est->tam_colunas[c] > 0) {
        int tam = est->tam_colunas[c];
        Carta t_col = est->colunas[c][tam - 1];
        if (pode_jogar_carta(est->descarte, t_col) == 1) {
            est->descarte = t_col;
            est->tam_colunas[c] = tam - 1; 
            movido = 1;
        }
    }
    return movido;
}

// Retira a carta do topo do stock e coloca-a como a nova carta de descarte 
int comprar_carta(JogoState *est) {
    int suc = 0;
    if (est != NULL && est->tam_stock > 0) {
        est->tam_stock--;
        est->descarte = est->stock[est->tam_stock];
        suc = 1;
    }
    return suc;
}

// O jogador vence se todas as colunas estiverem com tamanho zero (vazias) 
int jogo_ganho(const JogoState *est) {
    int vazio = 0, i = 0;
    if (est != NULL) {
        vazio = 1;
        while (i < NUM_COLUNAS && vazio == 1) {
            if (est->tam_colunas[i] > 0) {
                vazio = 0; 
            }
            i++;
        }
    }
    return vazio;
}

// Percorre as colunas para verificar se ainda existe algum movimento válido possível 
static int tem_jogada(const JogoState *est) {
    int possivel = 0, i = 0;
    while (i < NUM_COLUNAS && possivel == 0) {
        int tam = est->tam_colunas[i];
        if (tam > 0) {
            Carta topo_c = est->colunas[i][tam - 1];
            if (pode_jogar_carta(est->descarte, topo_c) == 1) {
                possivel = 1; 
            }
        }
        i++;
    }
    return possivel;
}

// O jogo termina em derrota se o stock acabar e não houver jogadas válidas nas colunas 
int jogo_perdido(const JogoState *est) {
    int perdido = 0;
    if (est != NULL && est->tam_stock == 0 && tem_jogada(est) == 0 && jogo_ganho(est) == 0) {
        perdido = 1;
    }
    return perdido;
}