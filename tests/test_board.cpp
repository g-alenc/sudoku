#include "../include/board.hpp"
#include "../include/cell.hpp"

#include <iostream>

using namespace std;

int main(){

    Board board;
    Cell& cell = board.grid[5][1];

    cout << cell.get_value() << endl;

    board.change_value(5, 1, 8);

    cout << cell.get_value() << endl;

    return 0;
}