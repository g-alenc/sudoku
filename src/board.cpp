#include "board.hpp"

Board::Board(){

}

void Board::reset_board(){
    for (auto line : board){
        for (Cell cell : line){
            if(cell.is_fixed()) cell.set_value(0);
        }
    }
}