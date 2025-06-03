#include "../include/board.hpp"
#include "../include/cell.hpp"
#include "../include/sudoku.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(){
    // cria o tabuleiro
    Sudoku game;

    // carrega um grid
    string grid_path = "tests/grid.txt";
    game.load_grid(grid_path);
    game.start_game();
    return 0;
}
