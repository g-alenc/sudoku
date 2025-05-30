#ifndef BOARD_HPP
#define BOARD_HPP
#include <array>
#include <string>
#include "cell.hpp"

using namespace std;

class Board{
    private:
        // array de arrays de celulas que formam o tabuleiro
        array<array<Cell, 9>, 9> grid;

    public:
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
        int change_value(int x, int y, int value);

        // carrega um grid de um arquivo .txt
        bool load_grid(string path);

};


#endif