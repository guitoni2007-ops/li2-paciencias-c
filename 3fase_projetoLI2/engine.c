#include <stdlib.h>
#include <string.h>
#include "engine.h"

// Adiciona carta ao fim da lista ligada.
void insercao_fim_lista(PilhaDinamica *p, Carta c) {
    NodoCarta *novo = malloc(sizeof(NodoCarta));
    novo->c = c; novo->prox = NULL;
    if (p->cartas == NULL) {
        p->cartas = novo;
    } else {
        NodoCarta *atual = p->cartas;
        while (atual->prox != NULL) atual = atual->prox;
        atual->prox = novo;
    }
    p->qtd++;
}

// Recolhe a carta a 'd' posições do fim. 1 = topo, qtd = ultima de todas.
Carta recolher_carta(const PilhaDinamica *p, int distancia_topo) {
    int cont = p->qtd - distancia_topo;
    NodoCarta *atual = p->cartas;
    while (cont > 0 && atual != NULL) {
        atual = atual->prox;
        cont--;
    }
    return atual->c;
}

// Liberta todos os nodos.
void libertar_pilha(PilhaDinamica *p) {
    NodoCarta *atual = p->cartas;
    while (atual != NULL) {
        NodoCarta *tmp = atual;
        atual = atual->prox;
        free(tmp);
    }
    p->cartas = NULL; p->qtd = 0;
}

// Anexa os nodos de backup (Deep Copy manual).
void espelho_pilha(PilhaDinamica *dest, const PilhaDinamica *orig) {
    libertar_pilha(dest);
    NodoCarta *atual = orig->cartas;
    while (atual != NULL) {
        insercao_fim_lista(dest, atual->c);
        atual = atual->prox;
    }
}

// Sub-função refatorada para prender
static void transacionar_lista(PilhaDinamica *pd, NodoCarta *corte) {
    if (pd->cartas == NULL) {
        pd->cartas = corte;
    } else {
        NodoCarta *t_pd = pd->cartas;
        while (t_pd->prox != NULL) t_pd = t_pd->prox;
        t_pd->prox = corte;
    }
}

// Sub-função divisão de lista.
static void fazer_corte(PilhaDinamica *po, PilhaDinamica *pd, int qtd) {
    int limit = po->qtd - qtd - 1, i = 0;
    NodoCarta *atual = po->cartas;
    while (i < limit) {
        atual = atual->prox;
        i++;
    }
    transacionar_lista(pd, atual->prox);
    atual->prox = NULL;
}

// Transfere N cartas da pilha O para a pilha D.
void ejecutar_movimento(EngineJogo *eng, int o, int d, int qtd) {
    PilhaDinamica *po = &eng->pilhas[o];
    PilhaDinamica *pd = &eng->pilhas[d];
    if (po->qtd == qtd) {
        transacionar_lista(pd, po->cartas);
        po->cartas = NULL;
    } else {
        fazer_corte(po, pd, qtd);
    }
    po->qtd -= qtd;
    pd->qtd += qtd;
}

// Tenta aplicar automáticos.
static int testar_q_para_regra(EngineJogo *eng, int o, int d, RegraMov reg) {
    int q = eng->pilhas[o].qtd;
    int movido = 0;
    while (q > 0 && !movido) {
        if (testar_regra_individual(&eng->pilhas[o], &eng->pilhas[d], q, reg.flags)) {
            ejecutar_movimento(eng, o, d, q);
            movido = 1;
        }
        q--;
    }
    return movido;
}

// Combina pares de pilhas com as regras de movimento.
static int verificar_regras_para_par(EngineJogo *eng, int o, int d) {
    int r = 0, movido = 0;
    while (r < eng->total_movimentos && !movido) {
        RegraMov reg = eng->movimentos[r];
        if (reg.is_auto && strcmp(eng->pilhas[o].nome_tipo, reg.origem) == 0 && 
            strcmp(eng->pilhas[d].nome_tipo, reg.destino) == 0) {
            movido = testar_q_para_regra(eng, o, d, reg);
        }
        r++;
    }
    return movido;
}

// Força pesquisa inteira.
static int tentar_um_auto(EngineJogo *eng) {
    int o = 0, movido = 0;
    while (o < eng->total_pilhas && !movido) {
        int d = 0;
        while (d < eng->total_pilhas && !movido) {
            if (o != d) movido = verificar_regras_para_par(eng, o, d);
            d++;
        }
        o++;
    }
    return movido;
}

// Loop mágico
void ejecutar_cadeia_auto(EngineJogo *eng) {
    int continuar = 1;
    while (continuar) continuar = tentar_um_auto(eng);
}

// Teste Win Cond
int verificar_vitoria(const EngineJogo *eng) {
    if (eng->total_vitoria == 0) return 0; // Previne vitoria instantanea nula
    int i = 0, ok = 1;
    while (i < eng->total_vitoria && ok) {
        RegraWin rw = eng->vitoria[i];
        int j = 0, conta = 0;
        while (j < eng->total_pilhas) {
            if (strcmp(eng->pilhas[j].nome_tipo, rw.tipo_pilha) == 0)
                conta += eng->pilhas[j].qtd;
            j++;
        }
        if (conta != rw.qtd_cartas) ok = 0; 
        i++;
    }
    return ok;
}

// Baralho Shuffle
static void colocar_deck(EngineJogo *eng, Carta *deck) {
    int idx = 0, j = 0;
    while (j < eng->total_pilhas) {
        int c = 0;
        libertar_pilha(&eng->pilhas[j]); // limpeza de inicio
        while (c < eng->pilhas[j].qtd_inicial) {
            insercao_fim_lista(&eng->pilhas[j], deck[idx]);
            idx++; c++;
        }
        j++;
    }
}

// Metodo auxiliar para preencher o deck base pela ordem certa
static void gerar_cartas_baralho(Carta *deck, int num_b) {
    int i = 0; Naipe naipes[4] = {PAUS, OUROS, COPAS, ESPADAS};
    while (i < num_b) {
        int n = 0;
        while (n < 4) {
            int v = 1;
            while (v <= 13) {
                deck[i*52 + n*13 + (v-1)] = (Carta){v, naipes[n]};
                v++;
            }
            n++;
        }
        i++;
    }
}

// Baralha o deck com base na seed dada pelo main de forma random
static void baralhar_deck(Carta *deck, int total, unsigned int seed) {
    srand(seed);
    int i = 0;
    while (i < total - 1) {
        int j = i + rand() / (RAND_MAX / (total - i) + 1);
        Carta tmp = deck[j]; deck[j] = deck[i]; deck[i] = tmp;
        i++;
    }
}

// Inicializa engine full.
void inicializar_baralhos(EngineJogo *eng, unsigned int seed) {
    int total = eng->num_baralhos * 52;
    Carta *deck = malloc(total * sizeof(Carta));
    
    gerar_cartas_baralho(deck, eng->num_baralhos);
    baralhar_deck(deck, total, seed);
    colocar_deck(eng, deck);
    free(deck);
}
