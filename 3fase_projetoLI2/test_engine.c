#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "engine.h"
#include "interface.h"

int carregar_configuracao(const char *path, EngineJogo *eng);
int testar_regra_individual(PilhaDinamica *orig, PilhaDinamica *dest, int qtd, const char *flags);
void insercao_fim_lista(PilhaDinamica *p, Carta c);

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

int main() {
    EngineJogo *eng = malloc(sizeof(EngineJogo));
    memset(eng, 0, sizeof(EngineJogo));
    carregar_configuracao("paciencias/klondikesimplificado.txt", eng);

    // Setup mock T3 and T5
    eng->pilhas[3].qtd = 0;
    insercao_fim_lista(&eng->pilhas[3], (Carta){10, PAUS});
    insercao_fim_lista(&eng->pilhas[3], (Carta){4, PAUS});
    insercao_fim_lista(&eng->pilhas[3], (Carta){10, ESPADAS});
    insercao_fim_lista(&eng->pilhas[3], (Carta){5, OUROS}); // 5♦ (tail)

    eng->pilhas[5].qtd = 0;
    insercao_fim_lista(&eng->pilhas[5], (Carta){2, PAUS});
    insercao_fim_lista(&eng->pilhas[5], (Carta){6, PAUS}); // 6♣ (tail)

    int valid = testar_se_regra_aplicavel(eng, 3, 5, 1);
    printf("Valid: %d\n", valid);

    // Also run testar_regra_individual directly
    int m_valid = testar_regra_individual(&eng->pilhas[3], &eng->pilhas[5], 1, "d<");
    printf("Direct d<: %d\n", m_valid);
    
    // Also run with D<
    int M_valid = testar_regra_individual(&eng->pilhas[3], &eng->pilhas[5], 1, "D<");
    printf("Direct D<: %d\n", M_valid);

    return 0;
}
