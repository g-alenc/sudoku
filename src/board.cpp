#include "board.hpp"
#include "cell.hpp"

Board::Board(){

}

void Board::reset_board(){
    for (auto line : grid){
        for (Cell cell : line){
            if(cell.is_fixed()) cell.set_value(0);
        }
    }
}
