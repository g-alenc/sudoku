#include "../include/cell.hpp"
#include "../include/json.hpp"
#include <stdexcept>

using namespace std;

Cell::Cell(){
    value = 0;
    fixed = false;
};

Cell::Cell(int value, bool fixed){
    this->value = value;
    this->fixed = fixed;
};

bool Cell::set_value(int value){
    
    if (this->fixed) return false;
    if (value < 0 | value > 9) return false;

    this->value = value;
    return true;
};

int Cell::get_value() const{
    return this->value;
};

bool Cell::is_fixed() const{
    return fixed;
}

void Cell::fix(){
    fixed = true;
}

void Cell::unfix(){
    fixed = false;
}
