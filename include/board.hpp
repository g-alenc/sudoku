#ifndef BOARD_HPP
#define BOARD_HPP
#include <array>
#include "cell.hpp"

using namespace std;

class Board{

    private:



    public:
        // lista das 81 células
        array<Cell, 81> board;

        // retorna o numero de células preenchidas
        int cells_filled() const;

        // retorna 'true' se o tabuleiro estiver completo
        bool is_completed() const;

        // apaga todas as células que tem 'fixed == ture'
        void reset_board();

        // acessa o valor de uma celula pela sua posição
        int value(int x, int y) const;

        // edita o valor de uma celula dada uma posição
        int chance_value(int x, int y, int value);

};


#endif