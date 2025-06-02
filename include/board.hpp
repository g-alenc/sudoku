#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>
#include <string>
#include "cell.hpp"

using namespace std;

class Board{
    private:
        // vector de vectors de celulas que formam o tabuleiro
        
        // transforma o grid em string
        string grid_to_string();
        
        public:
        Board();
        vector<vector<Cell>> grid;  
        
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

        //checa se o tabuleiro é valido com as ultimas funções
        bool is_valid_board();
};

#endif