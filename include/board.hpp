#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>
#include <string>
#include "cell.hpp"

using namespace std;

class Board{
    private:
        // vector de vectors de celulas que formam o tabuleiro
        vector<vector<Cell>> grid;  
        
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
        bool change_value(int x, int y, int value);

        vector<vector<Cell>> get_grid() const;
};

// Função para converter um objeto nlohmann::json para um objeto Board
inline void from_json(const nlohmann::json& j, Board& board) {

    auto grid = board.get_grid();
    j.at("grid").get_to(grid);

}


#endif