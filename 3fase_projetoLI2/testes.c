#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

// Verifica se as flags de naipe estao bem construidas com as letras corretas
void test_naipe_conversoes(void) {
    Naipe n; char c;
    CU_ASSERT_EQUAL(char_para_naipe('S', &n), 1);
    CU_ASSERT_EQUAL(n, ESPADAS);
    CU_ASSERT_EQUAL(naipe_para_char(COPAS, &c), 1);
    CU_ASSERT_EQUAL(c, 'H');
    CU_ASSERT_EQUAL(char_para_naipe('X', &n), 0);
}

// Testa toda a logica pesada de comparacao entre a carta topo e destination
void test_validador(void) {
    PilhaDinamica o, d;
    strcpy(o.nome_tipo, "TAB"); strcpy(d.nome_tipo, "TAB");
    o.cartas = NULL; o.qtd = 0;
    d.cartas = NULL; d.qtd = 0;

    insercao_fim_lista(&o, (Carta){2, ESPADAS});
    insercao_fim_lista(&o, (Carta){3, COPAS}); 
    insercao_fim_lista(&d, (Carta){4, ESPADAS}); 

    CU_ASSERT_EQUAL(testar_regra_individual(&o, &d, 1, "d<"), 1);
    CU_ASSERT_EQUAL(testar_regra_individual(&o, &d, 2, "+[d<"), 0);

    libertar_pilha(&o); libertar_pilha(&d);
}

// Testa a insercao e recolha de cartas na Pilha Dinamica
void test_listas_basico(void) {
    PilhaDinamica p;
    p.cartas = NULL; p.qtd = 0;
    insercao_fim_lista(&p, (Carta){1, COPAS});
    insercao_fim_lista(&p, (Carta){2, COPAS});
    CU_ASSERT_EQUAL(p.qtd, 2);
    Carta c = recolher_carta(&p, 1);
    CU_ASSERT_EQUAL(c.v, 2);
    libertar_pilha(&p);
    CU_ASSERT_EQUAL(p.qtd, 0);
}

// Testa a duplicacao do historico em espelho_pilha
void test_listas_espelho(void) {
    PilhaDinamica p1, p2;
    p1.cartas = NULL; p1.qtd = 0;
    p2.cartas = NULL; p2.qtd = 0;
    insercao_fim_lista(&p1, (Carta){10, ESPADAS});
    espelho_pilha(&p2, &p1);
    CU_ASSERT_EQUAL(p2.qtd, 1);
    libertar_pilha(&p1); libertar_pilha(&p2);
}

// Testa movimentacao logica da transacao
void test_movimento(void) {
    EngineJogo eng;
    eng.pilhas = malloc(2 * sizeof(PilhaDinamica));
    eng.total_pilhas = 2;
    eng.pilhas[0].cartas = NULL; eng.pilhas[0].qtd = 0;
    eng.pilhas[1].cartas = NULL; eng.pilhas[1].qtd = 0;
    insercao_fim_lista(&eng.pilhas[0], (Carta){1, COPAS});
    insercao_fim_lista(&eng.pilhas[0], (Carta){2, COPAS});
    ejecutar_movimento(&eng, 0, 1, 1);
    CU_ASSERT_EQUAL(eng.pilhas[0].qtd, 1);
    CU_ASSERT_EQUAL(eng.pilhas[1].qtd, 1);
    libertar_pilha(&eng.pilhas[0]); libertar_pilha(&eng.pilhas[1]);
    free(eng.pilhas);
}

// Valida logica de vitoria isolada
void test_vitoria(void) {
    EngineJogo eng;
    eng.total_pilhas = 1; eng.pilhas = malloc(sizeof(PilhaDinamica));
    strcpy(eng.pilhas[0].nome_tipo, "FUND");
    eng.pilhas[0].cartas = NULL; eng.pilhas[0].qtd = 1;
    eng.total_vitoria = 1; eng.vitoria = malloc(sizeof(RegraWin));
    strcpy(eng.vitoria[0].tipo_pilha, "FUND"); eng.vitoria[0].qtd_cartas = 1;

    CU_ASSERT_EQUAL(verificar_vitoria(&eng), 1);
    eng.pilhas[0].qtd = 0;
    CU_ASSERT_EQUAL(verificar_vitoria(&eng), 0);
    free(eng.pilhas); free(eng.vitoria);
}

// Cria blocos do CUNIT e arranca com eles
int main(void) {
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
    CU_pSuite s1 = CU_add_suite("Geral", 0, 0);
    CU_add_test(s1, "naipe", test_naipe_conversoes);
    CU_add_test(s1, "valida", test_validador);
    CU_add_test(s1, "motor_add", test_listas_basico);
    CU_add_test(s1, "motor_espelho", test_listas_espelho);
    CU_add_test(s1, "motor_mov", test_movimento);
    CU_add_test(s1, "motor_win", test_vitoria);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

// Tenta verificar o load the jogo gravado e seu parse
void test_save_load(void) {
    EngineJogo eng;
    eng.total_pilhas = 1; eng.pilhas = malloc(sizeof(PilhaDinamica));
    eng.pilhas[0].cartas = NULL; eng.pilhas[0].qtd = 0;
    strcpy(eng.pilhas[0].nome_tipo, "TAB");
    strcpy(eng.nome_jogo, "Teste");
    insercao_fim_lista(&eng.pilhas[0], (Carta){1, COPAS});
    CU_ASSERT_EQUAL(guardar_estado("tmpsav.txt", &eng), 1);
    
    eng.pilhas[0].qtd = 0; libertar_pilha(&eng.pilhas[0]);
    CU_ASSERT_EQUAL(carregar_estado("tmpsav.txt", &eng), 1);
    CU_ASSERT_EQUAL(eng.pilhas[0].qtd, 1);
    libertar_pilha(&eng.pilhas[0]); free(eng.pilhas);
}

// Testa os recursos do sistema undo se estao solidos
void test_gestor_undo(void) {
    GestorUndo *g = criar_sistema_undo(5);
    EngineJogo eng;
    eng.total_pilhas = 1; eng.pilhas = malloc(sizeof(PilhaDinamica));
    eng.pilhas[0].cartas = NULL; eng.pilhas[0].qtd = 0;
    
    registar_estado_undo(g, &eng);
    eng.pilhas[0].qtd = 10;
    CU_ASSERT_EQUAL(desfazer_jogada(g, &eng), 1);
    CU_ASSERT_EQUAL(eng.pilhas[0].qtd, 0);
    
    libertar_pilha(&eng.pilhas[0]); free(eng.pilhas); free(g->historico_estados); free(g);
}
