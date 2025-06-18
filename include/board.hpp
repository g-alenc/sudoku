#ifndef BOARD_HPP
#define BOARD_HPP
#include "../include/json.hpp"
#include <vector>
#include <string>
#include "cell.hpp"

using namespace std;

class Board{
    private:
        // vector de vectors de celulas que formam o tabuleiro
        
    public:
        // Da acesso das variaveis private às funções to_json() e from_json
        friend inline void to_json(nlohmann::json& j, const Board& board);
        friend inline void from_json(const nlohmann::json& j, Board& board);
    
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
        bool change_value(int x, int y, int value);

};

// Função para converter um objeto Board para um objeto nlohmann::json
inline void to_json(nlohmann::json& j, const Board& board) {

    j["grid"] = board.grid;

}

// Função para converter um objeto nlohmann::json para um objeto Board
inline void from_json(const nlohmann::json& j, Board& board) {

    j.at("grid").get_to(board.grid);

}

#endif