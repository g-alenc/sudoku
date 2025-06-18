#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include "board.hpp"
#include "interface.hpp"
#include <fstream>
#include "BoardGenerator.hpp"
#include <string>
#include <utility>
#include <vector>


struct Move {
    int x;
    int y;
    int old_value;
    int new_value;

    Move(int x, int y, int old_value, int new_value)
        : x(x), y(y), old_value(old_value), new_value(new_value) {}
};


// A classe 'Sudoku' orquestra o jogo, gerenciando o tabuleiro,
// a lógica de vitória e a interação com o usuário.
class Sudoku{
    private:
    // índice atual do movimento
    int current_move_index = -1;

    //histórico de movimentos, armazena os movimentos feitos no tabuleiro
    std::vector<Move> move_history;

    public:
    Board board;

    // ----------- Construtores -----------
    // Construtor: carrega um Sudoku a partir de um arquivo.
    Sudoku(const std::string& path);

    // Construtor: gera um novo Sudoku com uma dificuldade.
    Sudoku(BoardGenerator::Difficulty difficulty);

    // Construtor padrão: gera um Sudoku de dificuldade média.
    Sudoku();

    // ----------- Métodos de Gerenciamento do Jogo -----------
    // Gera um novo tabuleiro, substituindo o atual.
    void generate_new_board(BoardGenerator::Difficulty difficulty);

    //salva o estado atual do tabuleiro
    bool persist_grid(string path);

    //salva o estado atual do jogo
    bool save_game(const std::string& filename) const;
    
    bool load_game(const std::string& filename);

    // gera um novo jogo de acordo com uma dificuldade
    bool new_game(int diff);

    bool is_completed();

    //checa a caixa
    bool check_box(int n);
    // ----------- Métodos de Lógica e Validação -----------
    // Verifica se uma linha específica é válida.
    bool check_line(int n) const;

    // Verifica se uma coluna específica é válida.
    bool check_column(int n) const;

    // Verifica se uma caixa 3x3 específica é válida (o parâmetro n é o índice de 0 a 8).
    bool check_box(int n) const;

    // Verifica se o tabuleiro inteiro segue as regras do Sudoku.
    bool is_board_state_valid() const;
    
    // Verifica a condição de vitória: se o tabuleiro está completo e válido.
    bool is_solved() const;

    // ----------- Métodos de Interface com o Usuário -----------
    // Imprime o tabuleiro no terminal.
    void print_grid() const;

    // Inicia o loop principal do jogo no terminal.
    void start_game_loop();

    // Pede e retorna uma jogada do usuário.
    std::pair<std::pair<int, int>, int> get_move();

    // Aplica uma jogada do usuário ao tabuleiro.
    bool make_move(int x, int y, int value);
    
    const Board& get_board() const;

    //adiciona o movimento ao arquivo de log
    void add_move_to_log();
};

#endif // SUDOKU_HPP