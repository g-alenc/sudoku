#include "../include/json.hpp"
#include "board.hpp"
#include <iostream>
#include <fstream>  
#include <string>

Board::Board(){
    grid.resize(9);
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            Cell cell;
            grid[i].push_back(cell);
        }
    }
}

void Board::reset_board(){
    for (int x = 0; x < 9; x++){
        for (int y = 0; y < 9; y++){
            if(! grid[x][y].is_fixed()) grid[x][y].set_value(0);
        }
    }
}

bool Board::change_value(int x, int y, int value){
    // confere se a posição (x, y) é valida
    if (x < 0 || x > 8) return false;
    if (y < 0 || y > 8) return false;

    return grid[y][x].set_value(value);
}

int Board::get_value(int x, int y) const{
    return grid[x][y].get_value();
}

int Board::cells_filled() const{
    int counter = 0;
    
    for (auto& line : grid){
        for (Cell cell : line){
            if(cell.get_value() != 0) counter ++;
        }
    }

    return counter;
}

bool Board::is_completed() const{
    for (auto& line : grid){
        for (Cell cell : line){
            if (cell.get_value() == 0) return false;
        }
    }

    return true;
}

