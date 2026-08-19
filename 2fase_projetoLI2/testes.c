#include <CUnit/Basic.h>
#include "jogo.h"
#include "baralho.h"
#include "carta.h"
#include <string.h>

// Testa a conversão do valor e naipe da carta para uma string
void teste_carta_str() {
    Carta c;
    char buf[16];
    
    c.v = 1; c.n = ESPADAS;
    carta_para_str(&c, buf, sizeof(buf));
    CU_ASSERT_STRING_EQUAL(buf, "A♠");
    
    c.v = 11; c.n = ESPADAS;
    carta_para_str(&c, buf, sizeof(buf));
    CU_ASSERT_STRING_EQUAL(buf, "J♠");

    c.v = 12; c.n = COPAS;
    carta_para_str(&c, buf, sizeof(buf));
    CU_ASSERT_STRING_EQUAL(buf, "Q♥");

    c.v = 5; c.n = OUROS;
    carta_para_str(&c, buf, sizeof(buf));
    CU_ASSERT_STRING_EQUAL(buf, "5♦");

    c.v = 10; c.n = COPAS;
    carta_para_str(&c, buf, sizeof(buf));
    CU_ASSERT_STRING_EQUAL(buf, "10♥");
    
    c.v = 13; c.n = PAUS;
    carta_para_str(&c, buf, sizeof(buf));
    CU_ASSERT_STRING_EQUAL(buf, "K♣");
}

// Confirma se o baralho é inicializado em ordem
void teste_baralho_inicial() {
    Carta deck[52];
    baralho_inicial(deck);
    CU_ASSERT_EQUAL(deck[0].v, 1);
    CU_ASSERT_EQUAL(deck[0].n, PAUS);
    
    CU_ASSERT_EQUAL(deck[12].v, 13);
    CU_ASSERT_EQUAL(deck[12].n, PAUS);
    
    CU_ASSERT_EQUAL(deck[51].v, 13);
    CU_ASSERT_EQUAL(deck[51].n, ESPADAS);
}

// Verifica que embaralhar muda efetivamente a ordem com a mesma seed
void teste_baralho_embaralhado() {
    Carta deck1[52], deck2[52];
    int dif = 0, i = 0;
    
    baralho_inicial(deck1);
    baralho_inicial(deck2);
    baralho_embaralhado(deck2, 12345);
    
    while(i < 52) {
        if(deck1[i].v != deck2[i].v || deck1[i].n != deck2[i].n) {
            dif = 1;
        }
        i++;
    }
    CU_ASSERT_EQUAL(dif, 1); // Garante que a ordem não é a mesma
}

// Verifica que as cartas são retiradas sucessivamente ou recusadas caso esgote as 52
void teste_baralho_tira() {
    Carta deck[52], out;
    int topo = 0, suc = 0;
    baralho_inicial(deck);
    
    suc = baralho_tira(deck, &topo, &out);
    CU_ASSERT_EQUAL(suc, 1);
    CU_ASSERT_EQUAL(topo, 1);
    CU_ASSERT_EQUAL(out.v, 1);
    CU_ASSERT_EQUAL(out.n, PAUS);
    
    topo = 52;
    suc = baralho_tira(deck, &topo, &out);
    CU_ASSERT_EQUAL(suc, 0); // testar limites do baralho

    suc = baralho_tira(NULL, &topo, &out);
    CU_ASSERT_EQUAL(suc, 0);

    suc = baralho_tira(deck, NULL, &out);
    CU_ASSERT_EQUAL(suc, 0);

    suc = baralho_tira(deck, &topo, NULL);
    CU_ASSERT_EQUAL(suc, 0);
}

// Garante que o jogo distribui cartas com o número certo de cartas por coluna no arranque
void teste_inicializacao() {
    JogoState est;
    int suc = simon_inicializar(&est, 12345);
    CU_ASSERT_EQUAL(suc, 1);
    CU_ASSERT_EQUAL(est.tam_colunas[0], 8);
    CU_ASSERT_EQUAL(est.tam_colunas[3], 7);
    CU_ASSERT_EQUAL(est.tam_colunas[4], 6);
    CU_ASSERT_EQUAL(est.tam_colunas[9], 1);

    suc = simon_inicializar(NULL, 12345);
    CU_ASSERT_EQUAL(suc, 0);
}

// Testa limites, movimentos normais, saltos para colunas vazias e regras de empilhamento inválidas
void teste_mover() {
    JogoState est;
    simon_inicializar(&est, 123);
    
    // Configurar estado manipulado
    est.tam_colunas[0] = 2;
    est.colunas[0][0].v = 10; est.colunas[0][0].n = COPAS;
    est.colunas[0][1].v = 9;  est.colunas[0][1].n = COPAS;

    est.tam_colunas[1] = 1;
    est.colunas[1][0].v = 11; est.colunas[1][0].n = ESPADAS;

    // move 2 cartas bem combinadas de col 0 para 1
    int suc = simon_mover(&est, 0, 2, 1);
    CU_ASSERT_EQUAL(suc, 1);
    CU_ASSERT_EQUAL(est.tam_colunas[0], 0);
    CU_ASSERT_EQUAL(est.tam_colunas[1], 3);
    
    // movimento ilegal de naipe desencontrado internamente na sequencia
    est.tam_colunas[2] = 2;
    est.colunas[2][0].v = 8; est.colunas[2][0].n = COPAS;
    est.colunas[2][1].v = 7; est.colunas[2][1].n = OUROS; 
    int suc_fail = simon_mover(&est, 2, 2, 0); 
    CU_ASSERT_EQUAL(suc_fail, 0);

    // Teste de estado nulo (NULL pointer)
    int suc_null = simon_mover(NULL, 0, 1, 1);
    CU_ASSERT_EQUAL(suc_null, 0);
    
    // Fora dos limites (out of bounds)
    int suc_oob = simon_mover(&est, 10, 1, 1); // >9 cg
    CU_ASSERT_EQUAL(suc_oob, 0);
    
    suc_oob = simon_mover(&est, 0, 1, -1); // <0 cd
    CU_ASSERT_EQUAL(suc_oob, 0);
    
    // Quantidade inválida
    int suc_qt = simon_mover(&est, 0, 0, 1);
    CU_ASSERT_EQUAL(suc_qt, 0);
    
    suc_qt = simon_mover(&est, 1, 10, 2);
    CU_ASSERT_EQUAL(suc_qt, 0);
    
    // Movimento para coluna vazia
    est.tam_colunas[9] = 0;
    est.tam_colunas[8] = 1;
    est.colunas[8][0].v = 5; est.colunas[8][0].n = OUROS;
    int suc_empty = simon_mover(&est, 8, 1, 9);
    CU_ASSERT_EQUAL(suc_empty, 1);
    CU_ASSERT_EQUAL(est.tam_colunas[8], 0);
    CU_ASSERT_EQUAL(est.tam_colunas[9], 1);
    
    // Casamento de valores base inválido
    est.tam_colunas[4] = 1; est.colunas[4][0].v = 7; est.colunas[4][0].n = PAUS;
    est.tam_colunas[5] = 1; est.colunas[5][0].v = 5; est.colunas[5][0].n = PAUS;
    int suc_val_fail = simon_mover(&est, 5, 1, 4); // tentativa de pousar um 5 em cima de um 7
    CU_ASSERT_EQUAL(suc_val_fail, 0);
}

// Confirma se sequências inteiras são automaticamente limpas, testando a vitória
void teste_fundacoes() {
    JogoState est;
    simon_inicializar(&est, 123);
    
    est.tam_colunas[2] = 13;
    int v = 13, i = 0;
    while(i < 13) {
        est.colunas[2][i].v = v;
        est.colunas[2][i].n = PAUS;
        v--;
        i++;
    }
    
    simon_limpar_fundacoes(&est);
    CU_ASSERT_EQUAL(est.tam_colunas[2], 0);
    CU_ASSERT_EQUAL(est.fundacoes_concluidas, 1);
    
    int g = simon_jogo_ganho(&est);
    CU_ASSERT_EQUAL(g, 0);
    
    est.fundacoes_concluidas = 4;
    g = simon_jogo_ganho(&est);
    CU_ASSERT_EQUAL(g, 1);

    // Testes de ponteiros nulos
    simon_limpar_fundacoes(NULL); // verifica que a função não cai abruptamente
    g = simon_jogo_ganho(NULL);
    CU_ASSERT_EQUAL(g, 0);
}

// Cria as suites de testes do CUnit para validar toda a lógica a trabalhar em sintonia
int main() {
    CU_initialize_registry();
    
    CU_pSuite suite_carta = CU_add_suite("Suite Carta", 0, 0);
    CU_add_test(suite_carta, "teste_carta_str", teste_carta_str);
    
    CU_pSuite suite_baralho = CU_add_suite("Suite Baralho", 0, 0);
    CU_add_test(suite_baralho, "teste_baralho_inicial", teste_baralho_inicial);
    CU_add_test(suite_baralho, "teste_baralho_embaralhado", teste_baralho_embaralhado);
    CU_add_test(suite_baralho, "teste_baralho_tira", teste_baralho_tira);
    
    CU_pSuite suite_logic = CU_add_suite("Suite Jogo Logic", 0, 0);
    CU_add_test(suite_logic, "teste_inicializacao", teste_inicializacao);
    CU_add_test(suite_logic, "teste_mover", teste_mover);
    CU_add_test(suite_logic, "teste_fundacoes", teste_fundacoes);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
