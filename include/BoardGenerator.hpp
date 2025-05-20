#ifndef BOARDGENERATOR_HPP
#define BOARDGENERATOR_HPP

#include "board.hpp"

class BoardGenerator{
    private:


    public:
        // Gera um tabuleiro válido dada uma dificuldade
        Board generate_board(int difficulty);

};



#endif