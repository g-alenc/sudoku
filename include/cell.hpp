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
        // valores devem estar entre 1 e 9 (ou 0)
        // retorna true se a operação for feita com sucesso
        bool set_value(int value);

        // torna uma celula fixa
        bool is_fixed() const;

        // torna a célula fixa
        void fix();

        // desfixa uma célula
        void unfix();


        // Função para converter um objeto Cell para um objeto nlohmann::json
        inline void to_json(nlohmann::json& j, const Cell& cell) {
            j = nlohmann::json{
                {"value", cell.value}, 
                {"is_predefined", cell.fixed},
            };
        }


};


#endif