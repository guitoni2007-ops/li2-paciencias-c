#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "interface.h"

static int testar_se_regra_aplicavel(EngineJogo *eng, int o, int d, int q);
// Lista ficheiros numa pasta
static int contar_ficheiros_txt(const char *dir, char opcoes[50][MAX_STR]) {
    struct dirent *de;
    DIR *dr = opendir(dir);
    int cont = 0;
    if (dr == NULL) return -1;
    while ((de = readdir(dr)) != NULL) {
        if (strstr(de->d_name, "") != NULL && cont < 50) {
            strcpy(opcoes[cont], de->d_name);
            printf("[%d] %s\n", cont + 1, de->d_name);
            cont++;
        }
    }
    closedir(dr);
    return cont;
}

// Lida com o processamento da escolha introduzida
static int processar_escolha_valida(int escolha, int cont, const char *dir_paciencias, char opcoes[50][MAX_STR], char *ficheiro) {
    if (escolha > 0 && escolha <= cont) {
        sprintf(ficheiro, "%s/%s", dir_paciencias, opcoes[escolha - 1]);
        return 1;
    }
    return 0;
}

// Lista os ficheiros na pasta designada
int menu_paciencias(const char *dir_paciencias, char *ficheiro_escolhido) {
    char opcoes[50][MAX_STR];
    printf("=== ESCOLHA UMA PACIENCIA ===\n");
    int cont = contar_ficheiros_txt(dir_paciencias, opcoes);
    
    if (cont == -1) {
        printf("Aviso interno na dir '%s'.\n", dir_paciencias); return 0;
    }
    if (cont == 0) return 0;

    printf("Opcao: ");
    int escolha = 0;
    if (scanf("%d", &escolha) == 1) {
        return processar_escolha_valida(escolha, cont, dir_paciencias, opcoes, ficheiro_escolhido);
    }
    return 0;
}

// Converte numero para string especial
static void string_valor(int valor, char v[4]) {
    if (valor == 1) strcpy(v, "A");
    else if (valor == 11) strcpy(v, "J");
    else if (valor == 12) strcpy(v, "Q");
    else if (valor == 13) strcpy(v, "K");
    else sprintf(v, "%d", valor);
}

// Descobre se uma respetiva pilha possui a caracteristica de ocultacao visual
static int pilha_oculta(const EngineJogo *eng, int p_idx) {
    if (p_idx < 0 || p_idx >= eng->total_pilhas) return 0;
    const char *nt = eng->pilhas[p_idx].nome_tipo;
    int t = 0;
    while (t < eng->total_tipos) {
        if (strcmp(eng->tipos[t].nome, nt) == 0) {
            if (strchr(eng->tipos[t].flags, '_') != NULL) return 1;
        }
        t++;
    }
    return 0;
}

// Devolve a representacao unicode dos naipes para decorar o jogo no terminal com simbolos visuais
static const char* char_para_simbolo(char n) {
    if (n == 'H') return "♥";
    if (n == 'D') return "♦";
    if (n == 'S') return "♠";
    if (n == 'C') return "♣";
    return "?";
}

// Imprime formatado os componentes da fundacao para a consola
static void imprimir_componente_fund(Carta c, int escondido) {
    if (escondido) { printf("## "); return; }
    char n, v[4];
    naipe_para_char(c.n, &n);
    string_valor(c.v, v);
    if (n == 'H' || n == 'D') {
        printf("\033[31m%s%s\033[0m ", v, char_para_simbolo(n));
    } else {
        printf("%s%s ", v, char_para_simbolo(n));
    }
}

// Imprime pilhas Não-TAB (FUND, STOCK, DESC) na horizontal
static void desenhar_fundacoes_horizontais(const EngineJogo *eng) {
    int i = 0;
    while (i < eng->total_pilhas) {
        if (strcmp(eng->pilhas[i].nome_tipo, "TAB") != 0) {
            printf("%s %d: ", eng->pilhas[i].nome_tipo, i);
            int r = eng->pilhas[i].qtd, oc = pilha_oculta(eng, i);
            while (r > 0) {
                Carta c = recolher_carta(&eng->pilhas[i], r);
                imprimir_componente_fund(c, oc);
                r--;
            }
            printf("\n");
        }
        i++;
    }
}

// Junta as informacoes dos tabuleiros tudos para ser mais facil imprimir depois
static void compilar_iters(const EngineJogo *eng, NodoCarta **iters, int *ids_tab, int *nTabs, int *max_h, int *i_oc) {
    int i = 0;
    while (i < eng->total_pilhas) {
        if (strcmp(eng->pilhas[i].nome_tipo, "TAB") == 0) {
            ids_tab[*nTabs] = i;
            iters[*nTabs] = eng->pilhas[i].cartas;
            i_oc[*nTabs] = pilha_oculta(eng, i);
            if (eng->pilhas[i].qtd > *max_h) *max_h = eng->pilhas[i].qtd;
            (*nTabs)++;
        }
        i++;
    }
}

// Imprime formatado um elemento visual da arvore dinamica iterada 
static void imprimir_um_iter_formatado(NodoCarta *iter, int escondido) {
    if (escondido) { printf("  [##]  "); return; }
    char n, v[4];
    naipe_para_char(iter->c.n, &n);
    string_valor(iter->c.v, v);
    if (n == 'H' || n == 'D') {
        printf(" \033[31m[%2s%s]\033[0m ", v, char_para_simbolo(n));
    } else {
        printf(" [%2s%s] ", v, char_para_simbolo(n));
    }
}

// Imprime so uma linha horizontal do nosso tabuleiro nas colunas verticais
static void imprimir_linha_iters(NodoCarta **iters, int nTabs, int *i_oc) {
    int j = 0;
    while (j < nTabs) {
        if (iters[j] != NULL) {
            imprimir_um_iter_formatado(iters[j], i_oc[j]);
            iters[j] = iters[j]->prox;
        } else {
            printf("       ");
        }
        j++;
    }
    printf("\n");
}

// Processa o layout de varias colunas adjacentes e orquestra a impressao das listas na vertical
static void desenhar_tabuleiros_verticais(const EngineJogo *eng) {
    NodoCarta *iters[50];
    int ids_tab[50], ids_ocul[50];
    int nTabs = 0, i = 0, max_h = 0;
    
    compilar_iters(eng, iters, ids_tab, &nTabs, &max_h, ids_ocul);
    
    while (i < nTabs) { printf("  T%-2d  ", ids_tab[i]); i++; }
    printf("\n");
    int h = 0;
    while (h < max_h) {
        imprimir_linha_iters(iters, nTabs, ids_ocul);
        h++;
    }
}

// Funcao principal que reune a visualizacao das diferentes areas e compoe a interface inteira do jogo
void desenhar_tabuleiro(const EngineJogo *eng) {
    printf("\n--- %s ---\n", eng->nome_jogo);
    desenhar_fundacoes_horizontais(eng);
    printf("\n");
    desenhar_tabuleiros_verticais(eng);
    printf("----------------------------------\n");
}

// Trata do comando para o jogador guardar o estado
static int proc_save(EngineJogo *eng) {
    char arq[64], caminho_completo[256];
    printf("Nome ficheiro save: ");
    if (scanf("%63s", arq) == 1) {
        sprintf(caminho_completo, "paciencias/%s", arq);
        if (guardar_estado(caminho_completo, eng)) printf("Save OK.\n");
    }
    return 1;
}

// Trata do comando para o jogador conseguir voltar atras a ler de um file
static int proc_load(EngineJogo *eng) {
    char arq[64], caminho_completo[256];
    printf("Nome ficheiro load: ");
    if (scanf("%63s", arq) == 1) {
        sprintf(caminho_completo, "paciencias/%s", arq);
        if (carregar_estado(caminho_completo, eng)) printf("Load OK.\n");
        else printf("Load Falhou.\n");
    }
    return 1;
}

// Procura uma jogada valida percorrendo forcadamente combinacoes de jogo
static int procurar_dica(EngineJogo *eng, int o) {
    int d = 0;
    while (d < eng->total_pilhas) {
        if (o != d) {
            int q = eng->pilhas[o].qtd;
            while (q > 0) {
                if (testar_se_regra_aplicavel(eng, o, d, q)) {
                    printf("DICA: Tenta mover %d carta(s) de %d para %d!\n", q, o, d);
                    return 1;
                }
                q--;
            }
        }
        d++;
    }
    return 0;
}

// Inicializa a varredura pela primeira pilha que tenha cartas
static int proc_dica(EngineJogo *eng) {
    int o = 0;
    while (o < eng->total_pilhas) {
        if (eng->pilhas[o].qtd > 0 && procurar_dica(eng, o)) return 1;
        o++;
    }
    printf("DICA: Nenhuma jogada livre detetada.\n");
    return 1;
}

// Faz a deteccao das teclas atalho do teclado (undo, save, load, hint) para invocar funcoes globais do sistema
static int tratar_comando_extra(char c, EngineJogo *eng, GestorUndo *g) {
    int k = toupper((unsigned char)c);
    if (k == 'Q') return 0;
    if (k == 'U') {
        if (desfazer_jogada(g, eng)) printf("Undo Efetuado.\n");
        return 1;
    }
    if (k == 'S') return proc_save(eng);
    if (k == 'L') return proc_load(eng);
    if (k == 'H') return proc_dica(eng);
    return -1;
}

// Faz a triagem transacional iterando e enviando as cartas e contexto de origem/destino para subvalidador
static int testar_se_regra_aplicavel(EngineJogo *eng, int o, int d, int q) {
    int r = 0;
    while (r < eng->total_movimentos) {
        RegraMov reg = eng->movimentos[r];
        if (!reg.is_auto && strcmp(eng->pilhas[o].nome_tipo, reg.origem) == 0 && strcmp(eng->pilhas[d].nome_tipo, reg.destino) == 0) {
            if (testar_regra_individual(&eng->pilhas[o], &eng->pilhas[d], q, reg.flags)) return 1;
        }
        r++;
    }
    return 0;
}

// Sub-rotina segura: Regista salvaguarda no undo, valida as regras e no fim executa as listas ligadas
static void tentar_efetuar_movimento(EngineJogo *eng, GestorUndo *g, int o, int d, int q) {
    if (o < 0 || o >= eng->total_pilhas || d < 0 || d >= eng->total_pilhas) return;
    registar_estado_undo(g, eng);
    if (testar_se_regra_aplicavel(eng, o, d, q)) {
        ejecutar_movimento(eng, o, d, q);
        ejecutar_cadeia_auto(eng);
    } else {
        printf("Ilegal.\n");
        desfazer_jogada(g, eng);
    }
}

// Coracao visual de loop: Fica a guardar que o leitor passe comandos novos e envia para parsing
int pedir_input_utilizador(EngineJogo *eng, GestorUndo *g) {
    int d, q; char buffer[256];
    printf("Movimento O D Q | (U)ndo (S)ave (L)oad (H)int (Q)uit\n> ");
    if (scanf("%255s", buffer) != 1) return 1;
    int ext = tratar_comando_extra(buffer[0], eng, g);
    if (ext != -1) return ext;
    if (scanf("%d %d", &d, &q) == 2) tentar_efetuar_movimento(eng, g, atoi(buffer), d, q);
    return 1;
}
