#include "../include/board.hpp"
#include "../include/cell.hpp"

#include <iostream>

using namespace std;

int main(){

    Board board;
    Cell& cell = board.grid[5][1];

    cout << cell.get_value() << endl;

        for (auto& line : board.grid){
        for (Cell& cell : line){
            cell.set_value(5);
        }
    }

    cout << cell.get_value() << endl;
    
    cout << board.cells_filled() << endl;

    cout << board.is_completed() << endl;

    return 0;
}