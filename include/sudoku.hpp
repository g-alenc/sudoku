#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include "board.hpp"
#include "interface.hpp"


class Sudoku{
    private:
    // Interface Interface;
    
    // transforma o grid em string
    string grid_to_string();
    
    public:
    Board board;
    
    Sudoku(string path);
    Sudoku();

    // carrega um grid de um arquivo .txt
    bool load_grid(string path);
    
    //salva o estado atual do tabuleiro
    bool persist_grid(string path);
    
    //checa a linha
    bool check_line(int n);
    
    //checa a coluna
    bool check_column(int n);
    
    //checa a caixa
    bool check_box(int n);

    //checa se o tabuleiro é valido com as funções de check
    bool is_valid_board();

    //adiciona o movimento ao arquivo de log
    void add_move_to_log();

    //inicia o jogo
    void start_game();
};


#endif