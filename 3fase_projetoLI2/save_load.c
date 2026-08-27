#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"

// Converte um valor numérico ou letra para o formato interno de valor de carta.
static int obter_valor_especial(char v) {
    if (v == 'A') return 1;
    if (v == 'J') return 11;
    if (v == 'Q') return 12;
    if (v == 'K') return 13;
    return v - '0';
}

// Extrai a informação textual e preenche a estrutura Carta.
static int parse_carta_str(const char *str, Carta *c) {
    int tam = strlen(str);
    if (tam < 2) return 0;
    
    char n_c = str[tam - 1];
    char v_c = str[0];
    
    if (char_para_naipe(n_c, &c->n)) {
        if (tam == 3 && str[0] == '1' && str[1] == '0') c->v = 10;
        else c->v = obter_valor_especial(v_c);
        return 1;
    }
    return 0;
}

// Carrega as cartas de uma linha de um ficheiro gravado para uma PilhaDinamica.
void carregar_linha_pilha(char *linha, PilhaDinamica *p) {
    char token[MAX_STR];
    int pos = 0, offset = 0;
    
    while (sscanf(linha + pos, "%s%n", token, &offset) == 1) {
        Carta c;
        if (parse_carta_str(token, &c)) {
            insercao_fim_lista(p, c);
        }
        pos += offset;
    }
}


// Transforma valor interno e naipe da carta numa string
static void escrever_carta_ficheiro(FILE *f, Carta c) {
    char n_c, val_c[4];
    naipe_para_char(c.n, &n_c);
    
    if (c.v == 1) strcpy(val_c, "A");
    else if (c.v == 11) strcpy(val_c, "J");
    else if (c.v == 12) strcpy(val_c, "Q");
    else if (c.v == 13) strcpy(val_c, "K");
    else sprintf(val_c, "%d", c.v);
    
    fprintf(f, "%s%c ", val_c, n_c);
}

// Grava uma unica fila do tabuleiro guardando no documento
static void guardar_pilha_ficheiro(FILE *f, const EngineJogo *eng, int i) {
    int r = eng->pilhas[i].qtd;
    while (r > 0) {
        Carta c = recolher_carta(&eng->pilhas[i], r);
        escrever_carta_ficheiro(f, c);
        r--;
    }
    fprintf(f, "\n");
}

int guardar_estado(const char *caminho, const EngineJogo *eng) {
    FILE *f = fopen(caminho, "w");
    int i = 0;
    if (f == NULL) return 0;
    fprintf(f, "%s.paciencia\n", eng->nome_jogo);
    
    while (i < eng->total_pilhas) {
        guardar_pilha_ficheiro(f, eng, i);
        i++;
    }
    fclose(f);
    return 1;
}

// Metodo que destroi todas as cartas na mesa para meter as novas do load
static void limpar_todas_pilhas(EngineJogo *eng) {
    int i = 0;
    while (i < eng->total_pilhas) {
        libertar_pilha(&eng->pilhas[i]);
        eng->pilhas[i].qtd = 0;
        i++;
    }
}

// Le o resto das linhas correspondentes a cada pilha listada no txt
static void ler_pilhas_ficheiro(FILE *f, EngineJogo *eng) {
    char linha[512];
    int i = 0;
    while (i < eng->total_pilhas && fgets(linha, sizeof(linha), f)) {
        carregar_linha_pilha(linha, &eng->pilhas[i]);
        i++;
    }
}

// Carrega um ficheiro de save gravado no formato texto simples específico
int carregar_estado(const char *caminho, EngineJogo *eng) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;
    char linha[512];
    
    // Ignorar primeira linha (cabecalho do jogo)
    if (!fgets(linha, sizeof(linha), f)) { fclose(f); return 0; }
    
    limpar_todas_pilhas(eng);
    ler_pilhas_ficheiro(f, eng);
    
    fclose(f);
    return 1;
}
