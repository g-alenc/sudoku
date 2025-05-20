#include "../include/cell.hpp"
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

void Cell::set_value(int value){
    if (not fixed){
        // retorna um erro se o valor for menor do que 1 ou maior do que 9
        if (value < 1 || value > 9) throw runtime_error("Valor invalido para a célula");
        else this->value = value;
    }

    else{
        throw runtime_error("Tentando mudar o valor de uma célula fixa");
    }
};

int Cell::get_value() const{
    return this->value;
};