#ifndef CELL_HPP
#define CELL_HPP

class Cell{
    private:
        // valor da célula (0 representa uma célula vazia)
        int value;

        // 'true' se a célula for uma célula pré definida pelo tabuleiro
        bool fixed;

    public:
        Cell();

        Cell(int value, bool fixed);

        // retorna o valor da célula
        int get_value() const;

        // altera o valor da célula
        // se for uma célula fixa não deve altera-la
        // valores devem estar entre 1 e 9
        void set_value(int value);

        // torna a célula fixa
        void fix();

        // desfixa uma célula
        void unfix();
};


#endif