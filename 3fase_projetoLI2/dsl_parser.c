#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "engine.h"

// Limpa os comentários terminados com # e corta as quebras de linha no fim das strings.
void higienizar_linha(char *linha) {
    int i = 0;
    int fim = 0;
    while (linha[i] != '\0' && !fim) {
        if (linha[i] == '#' || linha[i] == '\n' || linha[i] == '\r') {
            linha[i] = '\0';
            fim = 1;
        }
        i++;
    }
}

// Regista as propriedades visuais de um novo tipo de pilha lido do parser.
static void add_tipo(EngineJogo *eng, const char *nome, const char *flags) {
    int idx = eng->total_tipos;
    eng->tipos = realloc(eng->tipos, (idx + 1) * sizeof(DefTipo));
    strcpy(eng->tipos[idx].nome, nome);
    strcpy(eng->tipos[idx].flags, flags);
    eng->total_tipos++;
}

// Cria e aloca dinamicamente o espaço para uma nova pilha do jogo.
static void add_init(EngineJogo *eng, const char *tipo, int qtd) {
    int idx = eng->total_pilhas;
    eng->pilhas = realloc(eng->pilhas, (idx + 1) * sizeof(PilhaDinamica));
    strcpy(eng->pilhas[idx].nome_tipo, tipo);
    eng->pilhas[idx].cartas = NULL; // Lista vazia inicialmente
    eng->pilhas[idx].qtd = 0;
    eng->pilhas[idx].qtd_inicial = qtd;
    eng->total_pilhas++;
}

// Adiciona uma nova regra de transação (manual ou automática) à lista das admitidas na DSL.
static void add_mov(EngineJogo *eng, const char *o, const char *d, const char *f, int is_a) {
    int idx = eng->total_movimentos;
    eng->movimentos = realloc(eng->movimentos, (idx + 1) * sizeof(RegraMov));
    strcpy(eng->movimentos[idx].origem, o);
    strcpy(eng->movimentos[idx].destino, d);
    strcpy(eng->movimentos[idx].flags, f);
    eng->movimentos[idx].is_auto = is_a;
    eng->total_movimentos++;
}

// Guarda os objetivos e as condições necessárias para vencer o cenário lido da DSL.
static void add_win(EngineJogo *eng, const char *tipo, int qtd) {
    int idx = eng->total_vitoria;
    eng->vitoria = realloc(eng->vitoria, (idx + 1) * sizeof(RegraWin));
    strcpy(eng->vitoria[idx].tipo_pilha, tipo);
    eng->vitoria[idx].qtd_cartas = qtd;
    eng->total_vitoria++;
}

// Processa tokens de definicao de metadados gerais do ficheiro
static void interp_meta_dados(int n, const char *cmd, const char *a1, EngineJogo *eng) {
    if (n >= 2) {
        if (strcmp(cmd, "JOGO") == 0) strcpy(eng->nome_jogo, a1);
        if (strcmp(cmd, "BARALHOS") == 0) eng->num_baralhos = atoi(a1);
    }
}

// Processa definicoes fisicas como tipos e pilhas iniciais
static void interp_elementos(int n, const char *cmd, const char *a1, const char *a2, EngineJogo *eng) {
    if (n >= 3) {
        if (strcmp(cmd, "TIPO") == 0) add_tipo(eng, a1, a2);
        if (strcmp(cmd, "INIT") == 0) add_init(eng, a1, atoi(a2));
        if (strcmp(cmd, "WIN") == 0) add_win(eng, a1, atoi(a2));
    }
}

// Lê e trata as informacoes base do setup (baralhos, jogo e tipos)
static void interpretar_setup(int n, const char *cmd, const char *a1, const char *a2, EngineJogo *eng) {
    interp_meta_dados(n, cmd, a1, eng);
    interp_elementos(n, cmd, a1, a2, eng);
}

// Analisa linhas do txt ligadas as regras dos movimentos validos
static void interpretar_mov(int n, const char *cmd, const char *a1, const char *a2, const char *a3, EngineJogo *eng) {
    if (n >= 4 && strcmp(cmd, "MOV") == 0) add_mov(eng, a1, a2, a3, 0);
    if (n >= 4 && strcmp(cmd, "AUTO") == 0) add_mov(eng, a1, a2, a3, 1);
}

// Analisa a linha do ficheiro via regex simples e encaminha a parametrização pela gramática DSL.
void interpretar_linha(char *linha, EngineJogo *eng) {
    char cmd[MAX_STR], a1[MAX_STR], a2[MAX_STR], a3[MAX_STR];
    int n = sscanf(linha, "%s %s %s %s", cmd, a1, a2, a3);
    interpretar_setup(n, cmd, a1, a2, eng);
    interpretar_mov(n, cmd, a1, a2, a3, eng);
}

// Abre o ficheiro de regras da paciência e lê o conteúdo sucessivo linha a linha.
int carregar_configuracao(const char *path, EngineJogo *eng) {
    char linha[256];
    FILE *f = fopen(path, "r");
    int ok = (f != NULL);
    while (ok && fgets(linha, sizeof(linha), f) != NULL) {
        higienizar_linha(linha);
        if (strlen(linha) > 0) {
            interpretar_linha(linha, eng);
        }
    }
    if (f != NULL) fclose(f);
    return ok;
}