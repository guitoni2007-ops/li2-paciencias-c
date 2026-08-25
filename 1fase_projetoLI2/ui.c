#include "ui.h"
#include "carta.h"
#include <stdio.h>

/* Implementação da interface textual. Não altera o estado além das chamadas
 * às funções do motor (jogar_carta_coluna, comprar_carta, etc.).
 */
int ui_run(JogoState *estado) {
    if (estado == NULL) return 1;

    int cmd = 0;
    char buf[16];

    while (cmd != 8 && !jogo_ganho(estado) && !jogo_perdido(estado)) {
        printf("\n--- GOLF SOLITAIRE ---\n");

        for (int i = 0; i < NUM_COLUNAS; ++i) {
            if (estado->tam_colunas[i] > 0) {
                carta_para_str(&estado->colunas[i][estado->tam_colunas[i]-1], buf, sizeof buf);
                printf("C%d: [%s] (%d cartas)\n", i, buf, estado->tam_colunas[i]);
            } else {
                printf("C%d: [---] (vazia)\n", i);
            }
        }

        carta_para_str(&estado->descarte, buf, sizeof buf);
        printf("\nDESCARTE: > %s <  |  STOCK: %d\n", buf, estado->tam_stock);
        printf("Comandos: 0-6 (coluna), 7 (biscar), 8 (sair)\n> ");

        if (scanf("%d", &cmd) != 1) {
            /* Limpa entrada inválida e continua */
            while (getchar() != '\n' && !feof(stdin));
            cmd = -1;
            continue;
        }

        if (cmd >= 0 && cmd <= 6) {
            jogar_carta_coluna(estado, cmd);
        } else if (cmd == 7) {
            comprar_carta(estado);
        }
    }

    if (cmd != 8) {
        if (jogo_ganho(estado)) {
            printf("\n--- PARABÉNS! GANHASTE! ---\n");
        } else {
            printf("\n--- JOGO TERMINADO: PERDESTE! ---\n");
        }
    } else {
        printf("\nJogo encerrado pelo utilizador.\n");
    }

    return 0;
}
