#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>
#include "cell.hpp"

using namespace std;

class Board{
    private:
        // vector de vectors de celulas que formam o tabuleiro
        
        public:
        vector<vector<Cell>> grid;  
        Board();
        
        // retorna o numero de células preenchidas
        int cells_filled() const;

        // retorna 'true' se o tabuleiro estiver completo
        bool is_completed() const;

        // apaga todas as células que tem 'fixed == false'
        void reset_board();

        // acessa o valor de uma celula pela sua posição
        int get_value(int x, int y) const;

        // edita o valor de uma celula dada uma posição
        void change_value(int x, int y, int value);

};

#endif