#ifndef INTERFACE_H
#define INTERFACE_H

#include "engine.h"

// Apresenta o tabuleiro baseando-se nas pilhas e suas visualizações (TIPO).
void desenhar_tabuleiro(const EngineJogo *eng);

// Trata do input na linha de comandos (CLI) para realizar as jogadas ou invocar menus/undo.
int pedir_input_utilizador(EngineJogo *eng, GestorUndo *g);

// Lista os ficheiros na pasta designada e pede ao utilizador para escolher.
int menu_paciencias(const char *dir_paciencias, char *ficheiro_escolhido);

#endif
