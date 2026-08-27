#ifndef ENGINE_H
#define ENGINE_H

#include "carta.h"

#define MAX_STR 64
#define MAX_FLAGS 32

// Guarda o nome de um tipo de pilha e as suas regras visuais (ex: se as cartas estão ocultas)
typedef struct {
    char nome[MAX_STR];
    char flags[MAX_FLAGS];
} DefTipo;

// Representação de um Nodo para a Lista Ligada de Cartas
typedef struct NodoCarta {
    Carta c;
    struct NodoCarta *prox;
} NodoCarta;

// Estrutura para controlar a lista ligada de cartas de cada pilha
typedef struct {
    char nome_tipo[MAX_STR];
    NodoCarta *cartas; // Aponta para o primeiro nodo(stack)
    int qtd;
    int qtd_inicial;
} PilhaDinamica;

// Guarda as informações de uma regra de movimento (de onde vem, para onde vai e restrições)
typedef struct {
    char origem[MAX_STR];
    char destino[MAX_STR];
    char flags[MAX_FLAGS];
    int is_auto;
} RegraMov;

// Define o objetivo de cartas necessário numa pilha para o jogo ser considerado ganho
typedef struct {
    char tipo_pilha[MAX_STR];
    int qtd_cartas;
} RegraWin;

// Estrutura central que armazena todo o estado do motor e as definições da paciência
typedef struct {
    char nome_jogo[MAX_STR];
    int num_baralhos;
    DefTipo *tipos;
    int total_tipos;
    PilhaDinamica *pilhas;
    int total_pilhas;
    RegraMov *movimentos;
    int total_movimentos;
    RegraWin *vitoria;
    int total_vitoria;
} EngineJogo;

int char_para_naipe(char c, Naipe *n);
int naipe_para_char(Naipe n, char *c);
int carregar_configuracao(const char *path, EngineJogo *eng);
int testar_regra_individual(PilhaDinamica *orig, PilhaDinamica *dest, int qtd, const char *flags);
void ejecutar_movimento(EngineJogo *eng, int o, int d, int qtd);
void ejecutar_cadeia_auto(EngineJogo *eng);
int verificar_vitoria(const EngineJogo *eng);
void inicializar_baralhos(EngineJogo *eng, unsigned int seed);

// Utilidades Linked List
void insercao_fim_lista(PilhaDinamica *p, Carta c);
Carta recolher_carta(const PilhaDinamica *p, int distancia_topo);
void espelho_pilha(PilhaDinamica *dest, const PilhaDinamica *orig);
void libertar_pilha(PilhaDinamica *p);

// Estruturas e assinaturas extra para undo e save/load
typedef struct {
    EngineJogo *historico_estados;
    int topo;
    int capacidade;
} GestorUndo;

int guardar_estado(const char *caminho, const EngineJogo *eng);
int carregar_estado(const char *caminho, EngineJogo *eng);
GestorUndo* criar_sistema_undo(int cap_inicial);
void registar_estado_undo(GestorUndo *g, const EngineJogo *estado_atual);
int desfazer_jogada(GestorUndo *g, EngineJogo *estado_atual);

#endif