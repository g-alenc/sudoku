#ifndef BOARDGENERATOR_HPP
#define BOARDGENERATOR_HPP

#include "board.hpp"
#include <random>
#include <vector>

class BoardGenerator {
public:
    // Definir a dificuldade do jogo
    enum class Difficulty {
        EASY,
        MEDIUM,
        HARD,
        MASTER
    };

    // Construtor
    BoardGenerator();

    Board generate(Difficulty difficulty);

private:
    // Funcoes auxiliares para a geracao


    bool find_empty_cell(const Board& board, int& row, int& col) const;
    bool is_safe_to_place(const Board& board, int row, int col, int num) const;
    bool solve_and_fill_board(Board& board_to_fill);
    void make_holes(Board& board_to_affect, int num_to_remove);
    int count_solutions(Board board_state); // Passa Board por valor para modificação local

    std::mt19937 rng; // Gerador de números aleatórios
};

#endif