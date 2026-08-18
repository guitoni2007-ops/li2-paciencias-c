# 🃏 Motor de Jogos de Paciência (Solitaire Engine) em C

> **Unidade Curricular:** Laboratórios de Informática II | **Universidade do Minho**  
> **Linguagem:** C  
> **Ambiente:** Linux (Ubuntu / Terminal)

---

### 📌 Sobre o Projeto
Desenvolvimento de um motor de jogo em C capaz de interpretar uma **Linguagem de Domínio Específico (DSL)** em ficheiros ASCII (`.paciencia`) para carregar, validar e jogar diferentes variantes de Paciência (como *Golf*, *Simple Simon*, *FreeCell* e *Klondike*)[cite: 1].

O projeto inclui um interpretador de regras, interface em linha de comandos (CLI), suporte para *Save/Load*, funcionalidade de *Undo* (voltar atrás) e validação automática de movimentos e condições de vitória[cite: 1].

---

### 🛠️ Principais Funcionalidades
* **Interpretador de DSL:** *Parser* dinâmico que lê ficheiros de configuração com a definição de baralhos, pilhas de cartas, restrições de movimento (`MOV`) e movimentações automáticas (`AUTO`)[cite: 1].
* **Gestão de Estado de Jogo:** Motor para execução das jogadas, suporte a histórico para *Undo* e persistência de dados (*Save/Load*)[cite: 1].
* **Validação de Regras:** Verificação rigorosa de naipes, cores, ordenação ascendente/descendente e capacidades das pilhas[cite: 1].
* **Interface CLI:** Interface em terminal interativa e separada da lógica de jogo[cite: 1].

---

### ⚙️ Engenharia de Software & Boas Práticas
O projeto foi desenvolvido seguindo métricas rigorosas de qualidade de código[cite: 1]:
* **Arquitetura Modular:** Separação estrita entre a camada de Lógica do Jogo e a Interface[cite: 1].
* **Testes Unitários:** Testes da lógica do programa implementados com a biblioteca `CUnit`[cite: 1].
* **Métricas de Qualidade (`pmccabe`):**
  * Complexidade ciclomática $\le 10$ por função[cite: 1].
  * Máximo de 15 instruções por função[cite: 1].
  * Zero utilização de variáveis globais[cite: 1].
  * Ausência de instruções de salto não estruturado (`goto`, `break`, `continue`)[cite: 1].

---

### 🚀 Como Compilar e Executar

1. **Clonar o repositório:**
   ```bash
   git clone [https://github.com/guitoni2007-ops/li2-paciencias-c.git](https://github.com/guitoni2007-ops/li2-paciencias-c.git)
   cd li2-paciencias-c
