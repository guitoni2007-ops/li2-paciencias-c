#ifndef UI_H
#define UI_H

#include "jogo.h"

/* Executa a interface textual do jogo.
 * Recebe um JogoState já inicializado e controla o loop de I/O.
 * Retorna 0 em saída normal, 1 em erro.
 */
int ui_run(JogoState *estado);

#endif /* UI_H */
