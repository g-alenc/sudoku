#include "BoardGenerator.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

BoardGenerator::BoardGenerator() : rng(std::random_device{}()) {
    //Inicia o geredador de numeros aleatorios
}

Board BoardGenerator::generate(Difficulty difficulty) {
    Board new_board; // Cria novo Board

    // Garantir que o Board comece limpo e nao fixo
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            new_board.grid[r][c].set_value(0);
            new_board.grid[r][c].unfix();
        }
    }

    // Preenche o Board com uma solucao correta
    if(!solve_and_fill_board(new_board)) {
        std::cerr << "Error (BoardGenerator): Could not generate a full Sudoku solution." << std::endl;
        return new_board;
    }

    // Determina quantos numeros remover 
    int numbers_remove;
    switch (difficulty) {
        case Difficulty::EASY:   numbers_remove = 30; break; 
        case Difficulty::MEDIUM: numbers_remove = 40; break;
        case Difficulty::HARD:   numbers_remove = 50; break;
        case Difficulty::MASTER: numbers_remove = 55; break;
        default:                 numbers_remove = 40;

    }

      // Remove os numeros
    make_holes(new_board, numbers_remove);

    // Fixa as celulas que restaram e garante que as celulas vazias nao estejam fixas
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (new_board.grid[r][c].get_value() != 0) {
                new_board.grid[r][c].fix();
            } else {
                new_board.grid[r][c].unfix(); // Garante que buracos nao sejam fixos
            }
        }
    }
    return new_board;
}

bool BoardGenerator::find_empty_cell(const Board& board, int& row, int& col) const {
    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            if (board.grid[row][col].get_value() == 0) return true;
        }
    }
    return false;
}

bool BoardGenerator::is_safe_to_place(const Board& board, int row, int col, int num) const {
    for (int c = 0; c < 9; ++c) { // Linha
        if (board.grid[row][c].get_value() == num) return false;
    }
    for (int r = 0; r < 9; ++r) { // Coluna
        if (board.grid[r][col].get_value() == num) return false;
    }
    int box_start_row = row - row % 3;
    int box_start_col = col - col % 3;
    for (int r_offset = 0; r_offset < 3; ++r_offset) { // Box 3x3
        for (int c_offset = 0; c_offset < 3; ++c_offset) {
            if (board.grid[box_start_row + r_offset][box_start_col + c_offset].get_value() == num) return false;
        }
    }
    return true;
}

bool BoardGenerator::solve_and_fill_board(Board& board_to_fill) {
    int row, col;
    if (!find_empty_cell(board_to_fill, row, col)) return true; // Preenchido

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(numbers.begin(), numbers.end(), this->rng);

    for (int num : numbers) {
        if (is_safe_to_place(board_to_fill, row, col, num)) {
            board_to_fill.grid[row][col].set_value(num); // Célula não está fixa aqui
            if (solve_and_fill_board(board_to_fill)) return true;
            board_to_fill.grid[row][col].set_value(0);
        }
    }
    return false;
}

void BoardGenerator::make_holes(Board& board_to_affect, int num_to_remove) {
    std::vector<std::pair<int, int>> positions;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            positions.push_back({r, c});
        }
    }
    std::shuffle(positions.begin(), positions.end(), this->rng);

    int holes_made = 0;
    for (const auto& pos : positions) {
        if (holes_made >= num_to_remove) break;
        int r = pos.first;
        int c = pos.second;
        if (board_to_affect.grid[r][c].get_value() == 0) continue;

        // int original_value = board_to_affect.grid[r][c].get_value();
        board_to_affect.grid[r][c].set_value(0);
        holes_made++;
    }
}

int BoardGenerator::count_solutions(Board board_state) {
    int row, col;
    if (!find_empty_cell(board_state, row, col)) return 1;
    int count = 0;
    for (int num = 1; num <= 9; ++num) {
        if (is_safe_to_place(board_state, row, col, num)) {
            board_state.grid[row][col].set_value(num);
            count += count_solutions(board_state);
            board_state.grid[row][col].set_value(0); 
            if (count > 1) return count;
        }
    }
    return count;
}