#include "../include/board.hpp"
#include "../include/cell.hpp"
#include <iostream>

using namespace std;

int main(){

    Board board;

    for (auto& line : board.grid){
        for (Cell& cell : line){
            cell.set_value(5);
        }
    }
    
    cout << board.cells_filled() << endl;
    cout << board.is_completed() << endl;
    
    board.reset_board();
    
    cout << board.cells_filled() << endl;
    cout << board.is_completed() << endl;

    return 0;
}
