#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include "board.hpp"
#include "interface.hpp"


class Sudoku{
    private:
        Board board;
        Interface Interface;

    public:
        // salva o tabuleiro em um arquivo externo
        void persist_board() const;
        
        //adiciona o movimento ao arquivo de log
        void add_move_to_log();

};


#endif