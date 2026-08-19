#include <stdlib.h>
#include <time.h>
#include "jogo.h"
#include "ui.h"

// Porta de entrada de todo o projeto onde criamos e iniciamos o jogo, além de passar o controlo logo a seguir para o que vai ser desenhado no ecrã.
int main(void) {
    JogoState jogo;

    // Apanha as horas do relógio do PC e usa-as para criar uma semente de baralhar as cartas num formato aleatório para ser diferente todas as vezes 
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);

    // Arranca a estrutura por debaixo do tapete (baralhar, distribuir cartas pelas colunas)
    if (!simon_inicializar(&jogo, seed)) {
        return 1; // Se algo de muito errado acontecer saímos com erro
    }

    // Depois das fundações prontas damos a vez ao código que vai pintar o ecrã para interagirmos com as cartas
    return ui_run(&jogo);
}
