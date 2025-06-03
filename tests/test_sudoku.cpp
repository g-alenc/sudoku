#include "../include/board.hpp"
#include "../include/cell.hpp"
#include "../include/sudoku.hpp"
#include <iostream>
#include <string>

using namespace std;

void print_grid(const Board& board) {
    const std::string RED = "\033[31m";    // Vermelho
    const std::string YELLOW = "\033[33m"; // Amarelo
    const std::string RESET = "\033[0m";   // Reseta a cor para o padrão do terminal

    std::cout << "\n\n---------------------" << std::endl;
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            if (y > 0 && y % 3 == 0) {
                std::cout << "| ";
            }

            const Cell& current_cell = board.grid[x][y];
            int value = current_cell.get_value();
            if (value == 0) {
                std::cout << "  "; 
            } else {
                if (current_cell.is_fixed()) {
                    std::cout << RED << value << RESET << " "; 
                } else {
                    std::cout << YELLOW << value << RESET << " ";
                }
            }
        }
        std::cout << std::endl;

        if ((x + 1) % 3 == 0 && x < 8) {
            std::cout << "---------------------" << std::endl;
        }
    }
    std::cout << "---------------------" << std::endl;
}

int main(){
    // cria o tabuleiro
    Sudoku game;

    // carrega um grid
    string grid_path = "tests/grid.txt";
    game.load_grid(grid_path);
    print_grid(game.board);

    for(int n = 0; n < 9; n++){   
        cout << "line " << n << ": " << game.check_line(n) << endl;
    }    

    //salva o grid
    game.persist_grid(grid_path);

    return 0;
}
