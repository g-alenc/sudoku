#include "board.hpp"
#include <fstream>  
#include <string>
#include <iostream>

using namespace std;

Board::Board(){

}

void Board::reset_board(){
    for (auto line : board){
        for (Cell cell : line){
            if(cell.is_fixed()) cell.set_value(0);
        }
    }
}

bool Board::load_grid(string path) {
    ifstream file;
    string grid_string;

    file.open(path);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << path << endl;
        return false;
    }

    getline(file, grid_string);

    if (grid_string.size() != 162) {
        cerr << "Error: Invalid grid string size. Expected 162, got " << grid_string.size() << endl;
        file.close();
        return false;
    }

    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            int pos_in_string = 2 * (9 * x + y); 

            char value_char = grid_string[pos_in_string];
            char fixed_char = grid_string[pos_in_string + 1];

            if (!isdigit(value_char)) {
                cerr << "Error: Invalid character '" << value_char << "' found at position "
                          << pos_in_string << ". Expected a digit (0-9)." << endl;
                file.close();
                return false;
            }

            int value = value_char - '0';
            grid[x][y].set_value(value);

            if (fixed_char == 'f') {
                grid[x][y].fix();
            }
        }
    }

    file.close();
    return true;
}
