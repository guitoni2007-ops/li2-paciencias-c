#include "engine.h"

// Converte os caracteres de texto representacionais da DSL para a Enumeração Interna Naipe original.
int char_para_naipe(char c, Naipe *n) {
    int sucesso = 1;
    if (c == 'C') *n = PAUS;
    else if (c == 'D') *n = OUROS;
    else if (c == 'H') *n = COPAS;
    else if (c == 'S') *n = ESPADAS;
    else sucesso = 0;
    return sucesso;
}

// Transmuta os dados enumerados de Naipe nos carateres para gravação em disco.
int naipe_para_char(Naipe n, char *c) {
    int sucesso = 1;
    if (n == PAUS) *c = 'C';
    else if (n == OUROS) *c = 'D';
    else if (n == COPAS) *c = 'H';
    else if (n == ESPADAS) *c = 'S';
    else sucesso = 0;
    return sucesso;
}