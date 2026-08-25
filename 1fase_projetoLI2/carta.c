#include "carta.h"
#include <stdio.h>

// Retorna o caractere especial correspondente ao símbolo do naipe 
static const char* obter_simbolo(Naipe n) {
    const char *simbolos[] = { "♣", "♦", "♥", "♠" };
    return simbolos[n];
}

// Formata a carta para uma string, convertendo números em letras (A, J, Q, K) quando necessário 
int carta_para_str(const Carta *c, char *buf, int bufsize) {
    const char *naipe_str = obter_simbolo(c->n);
    int res = 0;
    if (c->v == 1) res = snprintf(buf, bufsize, "A%s", naipe_str);
    else if (c->v == 11) res = snprintf(buf, bufsize, "J%s", naipe_str);
    else if (c->v == 12) res = snprintf(buf, bufsize, "Q%s", naipe_str);
    else if (c->v == 13) res = snprintf(buf, bufsize, "K%s", naipe_str);
    else res = snprintf(buf, bufsize, "%d%s", c->v, naipe_str);
    return res;
}