#include <stdlib.h>
#include <string.h>
#include "engine.h"

// Cria o gestor de histórico na inicialização do sistema.
GestorUndo* criar_sistema_undo(int cap) {
    GestorUndo *g = malloc(sizeof(GestorUndo));
    g->topo = 0;
    g->capacidade = cap;
    g->historico_estados = malloc(cap * sizeof(EngineJogo));
    return g;
}

// Helper para garantir e realocar capacidade quando o limite e excedido
static void verificar_capacidade(GestorUndo *g) {
    if (g->topo >= g->capacidade) {
        g->capacidade *= 2;
        g->historico_estados = realloc(g->historico_estados, g->capacidade * sizeof(EngineJogo));
    }
}

// Regista uma nova cópia profunda do estado antes de uma jogada ser efetuada.
void registar_estado_undo(GestorUndo *g, const EngineJogo *atual) {
    int i = 0;
    verificar_capacidade(g);
    g->historico_estados[g->topo] = *atual; // Shallow copy do esqueleto principal
    
    int t_pilhas = atual->total_pilhas;
    g->historico_estados[g->topo].pilhas = malloc(t_pilhas * sizeof(PilhaDinamica));
    
    while (i < t_pilhas) {
        g->historico_estados[g->topo].pilhas[i] = atual->pilhas[i];
        g->historico_estados[g->topo].pilhas[i].cartas = NULL;
        espelho_pilha(&g->historico_estados[g->topo].pilhas[i], &atual->pilhas[i]);
        i++;
    }
    g->topo++;
}

// Restaura o estado de jogo imediatamente anterior
int desfazer_jogada(GestorUndo *g, EngineJogo *atual) {
    if (g->topo > 0) {
        g->topo--;
        int i = 0;
        // Libertar actual memória!
        while (i < atual->total_pilhas) {
            libertar_pilha(&atual->pilhas[i]);
            i++;
        }
        free(atual->pilhas);
        
        *atual = g->historico_estados[g->topo];
        return 1;
    }
    return 0;
}
