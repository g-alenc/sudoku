#include "../include/json.hpp"
#include "board.hpp"
#include <iostream>
#include <fstream>  
#include <string>

Board::Board() {
    grid.resize(9);
    for (int i = 0; i < 9; i++) {
        grid[i].resize(9);
    }
}

void Board::reset_board() {
    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if (!grid[x][y].is_fixed()) {
                grid[x][y].set_value(0);
            }
        }
    }
}

bool Board::change_value(int x, int y, int value) {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
        return false;
    }
    // CORREÇÃO: Usando grid[x][y] para manter a consistência (x=linha, y=coluna).
    return grid[x][y].set_value(value);
}

int Board::get_value(int x, int y) const {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
        return -1; // Valor de erro
    }
    return grid[x][y].get_value();
}

int Board::cells_filled() const {
    int counter = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j].get_value() != 0) {
                counter++;
            }
        }
    }
    return counter;
}

bool Board::is_completed() const {
    return cells_filled() == 81;
}