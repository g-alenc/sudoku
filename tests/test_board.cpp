#include "../include/board.hpp"
#include "../include/cell.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(){

    Board board;

    string grid_path = "tests/grid.txt";
    board.load_grid(grid_path);
    
    cout << board.cells_filled() << endl;
    cout << board.is_completed() << endl;
    
    board.reset_board();
    
    cout << board.cells_filled() << endl;
    cout << board.is_completed() << endl;

    return 0;
}
