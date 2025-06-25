# Projeto sudoku🎮

Um projeto desenvolvido em C++ para disciplina Programação e Desenvolvimento de Software 2 - DCC204

## Principais Funcionalidades:

* Geração de dicas para o jogador.
* Botão para salvamento do jogo.
* Sistema de tempo de jogo, para cronometrar cada tabuleiro.
* Jogos com diferentes dificuldades.


## Estrutura do Projeto

* `include/`: contém as classes ou cabeçalho (arquivos `.hpp`);
* `src/`: são os arquivos para a implementação do código (`.cpp`);
* `Makefile`: arquivo para configuração do sistema de build;
* `README.md`: Este arquivo;
* `user_stories.md`: Histórias dos usuários com funcionalidades recomendadas;
* `crc_cards.md`: Cartóes CRC (Class Responsibility Card), utilizados para determinar as funcionalidades e dependências de cada classe;
* `.gitignore`: Arquivos ignorados pelo git.


## Requisitos: 

* **Compilador de C++:** 
    * g++ com C++11 ou superior.
* **Sistema Operacional:**
    * Testado em Windows e Linux (Windows 10, Windows 11, Ubuntu 20.04.06).

## Compilando e Executando com Makefile

1. Clone o repositório:
    ```bash
    git clone https://github.com/g-alenc/sudoku.git
    cd sudoku
    ```

2. Compile o projeto usando o Makefile:
    ```bash
    make
    ```

3. Execute o programa:
    ```bash
    ./build/my_program
    ```

4. Para limpar os arquivos de compilação, utilize:
    ```bash
    make clean
    ```

## Como jogar

1. Abra o terminal na pasta do projeto e execute:
    ```bash
    ./my_program
    ```
    (ou o nome do executável gerado pelo Makefile)

2. O servidor será iniciado e você verá uma mensagem como:
    ```
    C++ Sudoku Server listening on port 8080...
    Access the web interface at: http://localhost:8080/index.html
    Press Ctrl+C to stop the server.
    ```

3. Abra o navegador e acesse:
    ```
    http://localhost:8080/index.html
    ```

4. Jogue pelo navegador!
    - Use os botões para carregar um jogo salvo ou iniciar um novo jogo.
    - Escolha a dificuldade e jogue diretamente no navegador.

5. Para encerrar o servidor, pressione `Ctrl+C` no terminal.
