#include "../include/cell.hpp"
#include <iostream>

using namespace std;

int main(){

    Cell cell(5, false);

    cout << cell.get_value() << endl;

    cell.set_value(11);

    cout << cell.get_value() << endl;

    return 0;
}