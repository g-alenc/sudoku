#include "board.hpp"
#include <fstream>  
#include <string>
#include <iostream>

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

void Board::change_value(int x, int y, int value){
    grid[x][y].set_value(value);
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

string Board::grid_to_string(){
    string grid_string;

    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            int value = grid[x][y].get_value(); 
            grid_string = grid_string + to_string(value);

            string fixed = "l";
            if(grid[x][y].is_fixed()) fixed = "f";
            grid_string = grid_string + fixed;    
        }
    }
    return grid_string;
}

bool Board::persist_grid(string path){
    const string grid_string = grid_to_string();

    ofstream file(path);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << path << endl;
        return false;
    }

    file << grid_string << endl;


    file.close();
    return true;
}

