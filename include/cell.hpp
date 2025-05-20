#ifndef CELL_HPP
#define CELL_HPP

class Cell{
    private:
        // valor da célula (0 representa uma célula vazia)
        int value;

        // 'true' se a célula for uma célula pré definida pelo tabuleiro
        bool fixed;

    public:
        // retorna o valor da célula
        int check_value() const;

        // altera o valor da célula
        // se for uma célula fixa não deve altera-la
        void set_value();

};


#endif