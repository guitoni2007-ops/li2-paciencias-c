#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "engine.h"
#include "interface.h"

// Laco base do jogo: fica a pedir comandos infinitamente e testa vitorias
static void loop_jogo_paciencias(EngineJogo *motor, GestorUndo *gestor) {
    int a_jogar = 1;
    while (a_jogar) {
        desenhar_tabuleiro(motor);
        if (verificar_vitoria(motor)) {
            printf("!!! PARABENS !!! VENCESTE ESTA PACIENCIA !\n");
            a_jogar = 0;
        } else {
            a_jogar = pedir_input_utilizador(motor, gestor);
        }
    }
}

// Helper que cria e assinala variaveis padrao a 0 para preparar o engine
static EngineJogo* criar_motor() {
    EngineJogo *motor = malloc(sizeof(EngineJogo));
    motor->total_tipos = 0;
    motor->total_pilhas = 0;
    motor->total_movimentos = 0;
    motor->total_vitoria = 0;
    motor->tipos = NULL;
    motor->pilhas = NULL;
    motor->movimentos = NULL;
    motor->vitoria = NULL;
    return motor;
}

int main(void) {
    EngineJogo *motor = criar_motor();

    GestorUndo *gestor = criar_sistema_undo(100);
    char caminho[512];
    
    // Ler os cenários ASCII do diretorio definido globalmente
    if (!menu_paciencias("../paciencias", caminho) && !menu_paciencias("paciencias", caminho)) {
        printf("A encerar motor gral...\n");
        free(motor);
        return 1;
    }

    if (carregar_configuracao(caminho, motor)) {
        inicializar_baralhos(motor, time(NULL));
        ejecutar_cadeia_auto(motor);
        loop_jogo_paciencias(motor, gestor);
    } else {
        printf("Erro ao carregar o dsl: %s\n", caminho);
    }
    
    free(motor);
    return 0;
}
