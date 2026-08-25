#include <stdlib.h> /* srand */
#include <time.h>
#include "jogo.h"
#include "ui.h"

/* main: semeia, inicializa o jogo e delega a I/O para ui_run */
int main(void) {
    JogoState jogo;

    /* Semente centralizada: semear uma vez no início */
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);

    /* Inicializa o estado do jogo; passa seed por compatibilidade se necessário */
    if (!inicializar_jogo(&jogo, seed)) {
        return 1;
    }

    /* Executa a interface (loop de I/O) */
    return ui_run(&jogo);
}
