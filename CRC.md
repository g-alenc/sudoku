# Cartões CRC - Projeto Sudoku

## Classe: `Cell`
- **Responsabilidades:**
  - Armazenar o valor da célula (número entre 1 e 9, ou 0 se vazia).
  - Indicar se a célula é fixa (pré-definida no tabuleiro).
  - Permitir alterar o valor da célula, exceto se ela for fixa.
  - Permitir fixar ou liberar uma célula.
- **Colaboradores:**
  - `Board` (gerencia e utiliza as células).

---

## Classe: `Board`
- **Responsabilidades:**
  - Armazenar e organizar as células em uma matriz 9x9.
  - Permitir acessar e modificar o valor de uma célula específica.
  - Verificar se o tabuleiro está completo.
  - Contar quantas células estão preenchidas.
  - Resetar o tabuleiro (limpar células que não são fixas).
- **Colaboradores:**
  - `Cell` (compõe o tabuleiro).
  - `BoardGenerator` (gera o tabuleiro inicial).
  - `Sudoku` (controla o jogo utilizando o tabuleiro).

---

## Classe: `BoardGenerator`
- **Responsabilidades:**
  - Gerar um tabuleiro inicial de Sudoku válido, considerando o nível de dificuldade.
- **Colaboradores:**
  - `Board` (retorna um tabuleiro gerado e preenchido).

---

## Classe: `Interface`
- **Responsabilidades (em desenvolvimento):**
  - Gerenciar a interação com o usuário.
  - Exibir o tabuleiro e as opções.
  - Receber entradas do usuário (valores, posições, comandos).
- **Colaboradores:**
  - `Sudoku` (executa ações no jogo).
  - `Board` (exibe o estado atual do tabuleiro).

---

## Classe: `Sudoku`
- **Responsabilidades:**
  - Controlar o fluxo principal do jogo.
  - Gerenciar o tabuleiro e a interface.
  - Salvar o estado do tabuleiro em um arquivo externo (persistência).
  - Registrar os movimentos do jogador em um arquivo de log.
- **Colaboradores:**
  - `Board` (estado atual do jogo).
  - `Interface` (interação com o usuário).
  - `BoardGenerator` (para gerar tabuleiros).

---
