# 🃏 Motor de Jogos de Paciência (Solitaire Engine) -  Laboratórios de Informática II

[![Linguagem](https://img.shields.io/badge/Linguagem-C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.cppreference.com/w/c)
[![Gráficos](https://img.shields.io/badge/Gráficos-Terminal%20%2F%20CLI-lightgrey?style=for-the-badge)](https://en.wikipedia.org/wiki/Command-line_interface)

---

## 📌 Sobre o Projeto
Desenvolvimento de um motor de jogo em C capaz de interpretar uma Linguagem de Domínio Específico (DSL) em ficheiros ASCII (`.paciencia`) para carregar, validar e jogar diferentes variantes de Paciência (como Golf, Simple Simon, FreeCell e Klondike).

O projeto inclui um interpretador de regras, interface em linha de comandos (CLI), suporte para Save/Load, funcionalidade de Undo (voltar atrás) e validação automática de movimentos e condições de vitória.

---

## 🛠️ Principais Funcionalidades
* **Interpretador de DSL:** Parser dinâmico que lê ficheiros de configuração com a definição de baralhos, pilhas de cartas, restrições de movimento (MOV) e movimentações automáticas (AUTO).
* **Gestão de Estado de Jogo:** Motor para execução das jogadas, suporte a histórico para Undo e persistência de dados (Save/Load).
* **Validação de Regras:** Verificação rigorosa de naipes, cores, ordenação ascendente/descendente e capacidades das pilhas.
* **Interface CLI:** Interface em terminal interativa e separada da lógica de jogo.

---

## ⚙️ Engenharia de Software & Boas Práticas
O projeto foi desenvolvido seguindo métricas rigorosas de qualidade de código:

* **Arquitetura Modular:** Separação estrita entre a camada de Lógica do Jogo e a Interface.
* **Testes Unitários:** Testes da lógica do programa implementados com a biblioteca CUnit.
* **Métricas de Qualidade (pmccabe):**
  * Complexidade ciclomática <= 10 por função.
  * Máximo de 15 instruções por função.
  * Zero utilização de variáveis globais.
  * Ausência de instruções de salto não estruturado (`goto`, `break`, `continue`).

---

## 🚀 Como Compilar e Executar

### 📋 Pré-requisitos (Ubuntu / Linux)
Antes de compilar, garante que tens o compilador de C, o `make` e a biblioteca `CUnit` instalados:

```bash
sudo apt update
sudo apt install build-essential libcunit-dev
```

> 💡 **Nota:** Se queres apenas jogar a versão final com todas as funcionalidades (DSL, Save/Load e Undo), avança diretamente para a **Fase 3**. As Fases 1 e 2 estão no repositório como registo histórico do desenvolvimento.

---

### 📦 Passos para Execução

1. **Clonar o repositório:**
```bash
git clone [https://github.com/guitoni2007-ops/li2-paciencias-c.git](https://github.com/guitoni2007-ops/li2-paciencias-c.git)
cd li2-paciencias-c
```

2. **Navegar para a fase pretendida:**

* **Fase 1 (Lógica Inicial & Modelo de Cartas):**
```bash
cd 1fase_projetoLI2
make
./golf
```

* **Fase 2 (Motor de Regras & Interface CLI):**
```bash
cd 2fase_projetoLI2
make
./simon
```

* **Fase 3 (Versão Final: Interpretador DSL, Save/Load & Testes CUnit):**
```bash
cd 3fase_projetoLI2
make
./jogo_paciencias
```
