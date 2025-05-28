#include "board.hpp"
#include "cell.hpp"

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
    for (auto& line : grid){
        for (Cell& cell : line){
            if(! cell.is_fixed()) cell.set_value(0);
        }
    }
}

void Board::change_value(int x, int y, int value){
    grid[x][y].set_value(value);
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


