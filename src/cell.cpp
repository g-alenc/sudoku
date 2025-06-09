#include "cell.hpp"

Cell::Cell() {
    this->value = 0;
    this->fixed = false;
}

Cell::Cell(int value, bool fixed) {
    this->value = value;
    this->fixed = fixed;
}

bool Cell::set_value(int value) {
    if (this->fixed) {
        return false;
    }
    
    // CORREÇÃO: Usando '||' (OU lógico) para a verificação de intervalo.
    if (value < 0 || value > 9) {
        return false;
    }

    this->value = value;
    return true;
}

int Cell::get_value() const {
    return this->value;
}

bool Cell::is_fixed() const {
    return this->fixed;
}

void Cell::fix() {
    this->fixed = true;
}

void Cell::unfix() {
    this->fixed = false;
}
