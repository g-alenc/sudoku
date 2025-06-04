#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include "board.hpp"
#include "interface.hpp"
#include <fstream>
#include <utility>

class Sudoku{
    private:
    // Interface Interface;
    
    // transforma o grid em string
    string grid_to_string();
    
    public:
    Board board;
    
    // constructors
    Sudoku(string path);
    Sudoku();

    //gera um tabuleiro valido de acordo com a dificuldade passada (aceita valores entre 1 e 3)
    void generate_board(int diff);

    // carrega um grid de um arquivo .txt
    bool load_grid(string path);
    
    //salva o estado atual do tabuleiro
    bool persist_grid(string path);

    //salva o estado atual do jogo
    bool save_game(const std::string& filename) const;
    
    bool load_game(const std::string& filename);

    bool is_completed();

    //checa a linha
    bool check_line(int n);
    
    //checa a coluna
    bool check_column(int n);
    
    //checa a caixa
    bool check_box(int n);

    //checa se o tabuleiro é valido com as funções de check
    bool is_valid_board();

    //(terminal) mostra o estado atual do tabuleiro no terminal 
    void print_grid() const;
    
    //(terminal) inicia o jogo 
    void start_game();
    
    //(terminal) recebe uma jogada do terminal 
    pair<pair<int, int>, int> get_move();

    //aplica uma jogada ao grid
    bool make_move(int x, int y, int value);
    
    const Board& get_board() const;

    //adiciona o movimento ao arquivo de log
    void add_move_to_log();
};


#endif