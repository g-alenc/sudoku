# Projeto sudoku🎮

Um projeto desenvolvido em C++ para disciplina Programação e Desenvolvimento de Software 2 - DCC204

## Principais Funcionalidades:

* Geração de dicas para o jogador.
* Botão para salvamento do jogo.
* Sistema de tempo de jogo, para cronometrar cada tabuleiro.
* Jogos com diferentes dificuldades.
* Sistema para voltar jogadas.

## Estrutura do Projeto

* `include/`: contém as classes ou cabeçalho (arquivos `.hpp`);
* `src/`: são os arquivos para a implementação do código (`.cpp`);
* `Makefile`: arquivo para configuração do sistema build CMake;
* `README.md`: Este arquivo;
* `user_stories.md`: Histórias dos usuários com funcionalidades recomendadas;
* `crc_cards.md`: Cartóes CRC(Class Resposibility Card), utilizados parea determinar as funcionalidades e dependencias de cada classe;
* `.gitignore`: Arquivos ignorados pelo git.


## Requisitos: 

* **Compilador de C++:** 
    * g++ com C++11 ou superior.
* **Sistema Operacional:**
    * Testado em Windows e Linux (Windows 10, Windows 11, Ubuntu 20.04.06).
* **CMake** (Recomendado)
    * Arquivo disponível.


## Executando:

1. Clone o repositório:
    ```bash
    git clone https://github.com/g-alenc/sudoku.git
    cd \sudoku
    ```

2. Pasta build:
    ```bash
    mkdir build
    cd build
    ```

3. Cmake:
    ```bash
    cmake ..
    ```

4. Compilando:
    ```bash
    make
    #se utiliza o MingW: mingw32-make
    #se for Visual Studio Code, abra o projeto gerado pelo CMake e compile.
    ```

5. Executando:

    ```bash
    ./sudoku_jogo
    ```