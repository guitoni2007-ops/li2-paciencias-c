#ifndef CARTA_H
#define CARTA_H

// Definição dos quatro naipes possíveis 
typedef enum { PAUS, OUROS, COPAS, ESPADAS } Naipe;

// Estrutura base de uma carta: valor numérico e naipe 
typedef struct {
    unsigned char v; 
    Naipe n;
} Carta;

// Transforma os dados da estrutura em texto legível 
int carta_para_str(const Carta *c, char *buf, int bufsize);

#endif
