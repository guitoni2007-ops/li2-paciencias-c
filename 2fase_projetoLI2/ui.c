#include <stdio.h>
#include "ui.h"
#include "carta.h"

// Limpa os caracteres do teclado pendentes
static void limpa_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Imprime o texto inicial e a legenda numerada das dez colunas
static void imprime_cabecalho(const JogoState *estado) {
    int c = 0;
    printf("\n=== SIMPLE SIMON ===\n");
    printf("Fundações concluídas: %d\n\n", estado->fundacoes_concluidas);
    while (c < NUM_COLUNAS_SIMON) {
        printf("  C%d  ", c);
        c++;
    }
    printf("\n");
    c = 0;
    while (c < NUM_COLUNAS_SIMON) {
        printf("----- ");
        c++;
    }
    printf("\n");
}

// Calcula qual é a coluna com maior número de cartas de momento
static int encontra_max_h(const JogoState *estado) {
    int max_h = 0, c = 0;
    while (c < NUM_COLUNAS_SIMON) {
        if (estado->tam_colunas[c] > max_h) {
            max_h = estado->tam_colunas[c];
        }
        c++;
    }
    return max_h;
}

// Desenha a grelha de jogo imprimindo cartas linha a linha para formarem colunas visuais
static void imprime_cartas(const JogoState *estado) {
    char buf[16];
    int max_h = encontra_max_h(estado);
    int l = 0;
    while (l < max_h) {
        int c = 0;
        while (c < NUM_COLUNAS_SIMON) {
            if (l < estado->tam_colunas[c]) {
                carta_para_str(&estado->colunas[c][l], buf, sizeof(buf));
                if (estado->colunas[c][l].v == 10) {
                    printf("[%s] ", buf); // "10" mais o naipe = 3 caracteres visuais
                } else {
                    printf("[ %s] ", buf); // "A", "K" ou número simples mais naipe = 2 caracteres visuais (+1 espaço para compensar)
                }
            } else {
                printf("      "); // Espaço vazio na coluna tem de ocupar exatamente os mesmos 6 espaços
            }
            c++;
        }
        printf("\n");
        l++;
    }
    printf("\n");
}

// Sub-função para ler os outros dois argumentos e aplicar regras da jogada
static void ler_resto_comando(JogoState *estado, int cmd) {
    int qty = 0, dest = 0;
    if (scanf("%d %d", &qty, &dest) != 2) {
        limpa_stdin();
        printf("Formato errado! Tenta: [Origem] [Qtd] [Destino]\n");
    } else {
        limpa_stdin();
        if (simon_mover(estado, cmd, qty, dest)) {
            simon_limpar_fundacoes(estado); // Arruma o conjunto se formar sequencia rei-as
        } else {
            printf("\n>>> Jogada não permitida pelas regras! Tenta de novo.\n");
        }
    }
}

// Pede input ao utilizador e distribui a execução apropriada
static int processar_jogada(JogoState *estado) {
    int cmd = 0, sair = 0;
    printf("\n[ INSTRUÇÕES ] Mover cartas: <Origem> <Quantidade> <Destino> | Sair: -1\n");
    printf("A tua jogada (exemplo: '2 1 3'): ");
    if (scanf("%d", &cmd) != 1) {
        limpa_stdin();
        printf("Erro a ler o comando!\n");
    } else if (cmd == -1) {
        sair = 1;
    } else {
        ler_resto_comando(estado, cmd);
    }
    return sair;
}

// Executa o loop principal 
int ui_run(JogoState *estado) {
    int sair = 0, erro = 0;
    if (estado == NULL) {
        erro = 1;
    } else {
        while (!simon_jogo_ganho(estado) && sair == 0) {
            imprime_cabecalho(estado);
            imprime_cartas(estado);
            sair = processar_jogada(estado);
        }
        if (simon_jogo_ganho(estado)) {
            printf("\n=== PARABÉNS! VENCESTE O JOGO! ===\n");
        } else {
            printf("\nSaíste do jogo.\n");
        }
    }
    return erro;
}
