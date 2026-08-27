#include <string.h>
#include "engine.h"

// Retorna 1 (vermelho) ou 0 (preto) para validação de cores alternativas
static int obter_cor(Naipe n) {
    int cor = 0;
    if (n == OUROS || n == COPAS) cor = 1;
    return cor;
}

// Valida lógicas de adjacência numa sequência (cores, alternância, n-1, n+1)
static int validar_propriedade_par(Carta c1, Carta c2, char f) {
    int ok = 1;
    if (f == 'm' && c1.n != c2.n) ok = 0;
    if (f == 'd' && obter_cor(c1.n) == obter_cor(c2.n)) ok = 0;
    if (f == '<' && c1.v != (c2.v - 1)) ok = 0;
    if (f == '>' && c1.v != (c2.v + 1)) ok = 0;
    return ok;
}

// Garante que uma sequência de N cartas respeita a restrição 'f' (ex: alterna cores)
static int validar_seq_flag(PilhaDinamica *p, int qtd, char f) {
    int i = qtd; int ok = 1;
    while (i > 1 && ok) {
        Carta c1 = recolher_carta(p, i);
        Carta c2 = recolher_carta(p, i - 1);
        ok = validar_propriedade_par(c1, c2, f);
        i--;
    }
    return ok;
}

// Interpreta as condicoes estruturais da sequencia ([)
static int validar_estrutura_regras(PilhaDinamica *p, int qtd, const char *flags) {
    int i = 0, tem_mais = 0, ok = 1;
    while (flags[i] != '\0') {
        if (flags[i] == '[') tem_mais = 1;
        if (flags[i] == '+') {
            if (qtd > 1 && !tem_mais) ok = 0;
        } else if (flags[i] != '[' && tem_mais) {
            if (!validar_seq_flag(p, qtd, flags[i])) ok = 0;
        }
        i++;
    }
    return ok;
}

// Testa o encaixe na fundação/vazia perante char identificador
// Funcao auxiliar para validar as letras (Ases, Reis, coringa)
static int val_letra(char f, Carta t_seq, Carta f_seq, int d_vazia) {
    if (f == '*') return 1;
    if (f == 'a') return (t_seq.v == 1);
    if (f == 'A') return (f_seq.v == 1);
    if (f == 'k') return (t_seq.v == 13);
    if (f == 'K') return (f_seq.v == 13);
    if (f == 'V') return d_vazia;
    return -1;
}

// Verifica regras matematicas como por exemplo o topo ser o N - 1
static int val_relacional(char f, Carta f_seq, Carta t_des, int d_vazia) {
    if (f == '<') return (d_vazia ? 0 : (f_seq.v == t_des.v - 1));
    if (f == '>') return (d_vazia ? 0 : (f_seq.v == t_des.v + 1));
    if (f == '~') return (d_vazia ? 0 : (f_seq.v == t_des.v - 1 || f_seq.v == t_des.v + 1));
    return -1;
}

// Helper para checar a logica de cores alternadas ou naipes iguais
static int val_cor(char f, Carta f_seq, Carta t_des, int d_vazia) {
    if (f == 'M') return (d_vazia ? 0 : (f_seq.n == t_des.n));
    if (f == 'X') return (d_vazia ? 0 : (f_seq.n != t_des.n));
    if (f == 'D') return (d_vazia ? 0 : (obter_cor(f_seq.n) != obter_cor(t_des.n)));
    return -1;
}

// Encaminha a verificacao para a sub-rotina responsavel pelo respetivo teste (letra, relacional ou cor).
static int validar_flag_cond(char f, Carta t_seq, Carta f_seq, Carta t_des, int d_vazia) {
    int res = val_letra(f, t_seq, f_seq, d_vazia);
    if (res != -1) return res;
    
    res = val_relacional(f, f_seq, t_des, d_vazia);
    if (res != -1) return res;

    res = val_cor(f, f_seq, t_des, d_vazia);
    if (res != -1) return res;

    return 1;
}

// Faz o loop pela string de flags e chama a validacao individual
static int verificar_comp_loop(const char *flags, Carta t_seq, Carta f_seq, Carta t_des, int v_dest) {
    int i = 0, compativel = 1;
    while (flags[i] != '\0') {
        if (!validar_flag_cond(flags[i], t_seq, f_seq, t_des, v_dest)) {
            compativel = 0;
        }
        i++;
    }
    return compativel;
}

// Agregador de regras: combina testes singulares e de sequência
int testar_regra_individual(PilhaDinamica *orig, PilhaDinamica *dest, int qtd, const char *flags) {
    if (orig->qtd < qtd) return 0;
    int v_dest = (dest->qtd == 0);
    Carta t_seq = recolher_carta(orig, 1);
    Carta f_seq = recolher_carta(orig, qtd);
    Carta t_des = (v_dest) ? t_seq : recolher_carta(dest, 1); 

    if (v_dest && strchr(flags, '*') == NULL && strchr(flags, 'V') == NULL) return 0;

    int comp = verificar_comp_loop(flags, t_seq, f_seq, t_des, v_dest);
    if (comp) comp = validar_estrutura_regras(orig, qtd, flags);
    return comp;
}
