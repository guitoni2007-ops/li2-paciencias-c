#include "jogo.h"
#include <stddef.h>

// Função auxiliar para tirar as cartas do baralho e pô-las numa coluna específica até encher a quantidade pretendida
static void preenche_cols(JogoState *est, Carta deck[MAX_CARTAS_SIMON], int *t, int c, int q) {
    int l = 0;
    while (l < q) {
        baralho_tira(deck, t, &est->colunas[c][l]);
        l++;
    }
    est->tam_colunas[c] = q;
}

// Trata da primeira fase do jogo onde 3 das colunas recebem 8 cartas cada uma
static void distribui_bloco1(JogoState *est, Carta deck[52], int *t) {
    int c = 0;
    while (c < 3) {
        preenche_cols(est, deck, t, c, 8);
        c++;
    }
}

// Trata do resto das colunas, dando progressivamente cada vez menos cartas à medida que avança
static void distribui_resto(JogoState *est, Carta deck[52], int *t) {
    int c = 3, q = 7;
    while (c < NUM_COLUNAS_SIMON) {
        preenche_cols(est, deck, t, c, q);
        c++;
        q--;
    }
}

// Começa um jogo : cria um baralho, baralha, e reparte as cartas pelas várias colunas 
int simon_inicializar(JogoState *estado, unsigned int seed) {
    Carta deck[52];
    int topo = 0, suc = 0;
    if (estado != NULL) {
        baralho_inicial(deck);
        baralho_embaralhado(deck, seed);
        estado->fundacoes_concluidas = 0;
        distribui_bloco1(estado, deck, &topo);
        distribui_resto(estado, deck, &topo);
        suc = 1;
    }
    return suc;
}

// Confirma apenas se um naipe de uma carta coincide com o da outra e se os valores formam uma escada descendente
static int eq_naipes(Carta superior, Carta inferior) {
    int eq = 0;
    if (superior.n == inferior.n && superior.v == inferior.v + 1) {
        eq = 1;
    }
    return eq;
}

// Percorre um conjunto de cartas de uma coluna para confirmar se elas formam uma sequência válida para podermos movimentá-las juntas
static int testa_seq(const JogoState *est, int c, int start) {
    int max = est->tam_colunas[c];
    int i = start, val = 1;
    while (i < max - 1 && val == 1) {
        if (eq_naipes(est->colunas[c][i], est->colunas[c][i+1]) == 0) {
            val = 0;
        }
        i++;
    }
    return val;
}

// Vê se a carta que queremos jogar (co) pode assentar em cima do topo da coluna de destino (cd)
static int testa_encaixe(Carta co, Carta cd) {
    int val = 0;
    if (cd.v == co.v + 1) {
        val = 1;
    }
    return val;
}

// Realiza a verdadeira ação de copiar cartas da coluna origem para o fim da coluna de destino, e depois atualiza o tamanho das duas
static void faz_move(JogoState *est, int cg, int cd, int qt) {
    int i = 0, pto = est->tam_colunas[cg] - qt;
    int ptd = est->tam_colunas[cd];
    while (i < qt) {
        est->colunas[cd][ptd+i] = est->colunas[cg][pto+i];
        i++;
    }
    est->tam_colunas[cg] -= qt;
    est->tam_colunas[cd] += qt;
}

// Garante que a coluna pedida nas jogadas existe (ou seja, é entre 0 e 9) para o jogo não rebentar
static int check_bounds(int c) {
    int val = 0;
    if (c >= 0 && c < NUM_COLUNAS_SIMON) val = 1;
    return val;
}

// Confirma de cima a baixo se os detalhes introduzidos na jogada não violam o tamanho limite das colunas
static int args_valid(const JogoState *est, int cg, int qt, int cd) {
    int v = 0;
    if (est != NULL) {
        if (check_bounds(cg) == 1 && check_bounds(cd) == 1) {
            if (qt > 0 && qt <= est->tam_colunas[cg]) {
                v = 1;
            }
        }
    }
    return v;
}

// Coração das jogadas no Simon: junta tudo para só deixar mover se uma sequência for válida e se a carta do topo assentar na base do destino
int simon_mover(JogoState *est, int cg, int qt, int cd) {
    int suc = 0;
    if (args_valid(est, cg, qt, cd) == 1) {
        int st = est->tam_colunas[cg] - qt;
        if (testa_seq(est, cg, st) == 1) {
            Carta co = est->colunas[cg][st];
            if (est->tam_colunas[cd] == 0) {
                // Se o destino está vazio podemos meter qualquer coisa
                suc = 1;
            } else {
                Carta td = est->colunas[cd][est->tam_colunas[cd]-1];
                if (testa_encaixe(co, td) == 1) suc = 1;
            }
        }
    }
    if (suc == 1) faz_move(est, cg, cd, qt);
    return suc;
}

// Procura numa coluna ver se lá está uma escada de Reis até Ases (13 cartas) arrumadas do mesmo naipe
static int test_ka(JogoState *est, int c) {
    int t = est->tam_colunas[c], ret = 0;
    if (t >= 13) {
        int st = t - 13;
        if (est->colunas[c][st].v == 13) {
            ret = testa_seq(est, c, st);
        }
    }
    return ret;
}

// Faz uma varridela por todas as colunas; se apanhar alguma já em perfeitas condições (do Rei ao Ás do mesmo naipe) arruma-a permanentemente para o monte
void simon_limpar_fundacoes(JogoState *estado) {
    int c = 0;
    if (estado != NULL) {
        while (c < NUM_COLUNAS_SIMON) {
            if (test_ka(estado, c) == 1) {
                estado->tam_colunas[c] -= 13;
                estado->fundacoes_concluidas++;
            }
            c++;
        }
    }
}

// Confirma se as quatro sequências de Rei até Ás já estão completas e fechadas para festejar a vitória
int simon_jogo_ganho(const JogoState *estado) {
    int g = 0;
    if (estado != NULL && estado->fundacoes_concluidas == 4) {
        g = 1;
    }
    return g;
}