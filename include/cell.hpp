#ifndef CELL_HPP
#define CELL_HPP
#include "../include/json.hpp"

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
        // valores devem estar entre 1 e 9 (ou 0)
        // retorna true se a operação for feita com sucesso
        bool set_value(int value);

        // torna uma celula fixa
        bool is_fixed() const;

        // torna a célula fixa
        void fix();

        // desfixa uma célula
        void unfix();



};

// Função para converter um objeto Cell para um objeto nlohmann::json
inline void to_json(nlohmann::json& j, const Cell& cell) {
    j = nlohmann::json{
        {"value", cell.get_value()},
        {"is_fixed", cell.is_fixed()},
    };
}

// Função para converter um objeto nlohmann::json para um objeto Cell
inline void from_json(const nlohmann::json& j, Cell& cell) {
    int val = cell.get_value();
    bool fixed = cell.is_fixed();

    j.at("value").get_to(val);
    j.at("is_fixed").get_to(fixed);

    cell = Cell(val, fixed);
}

#endif