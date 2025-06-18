#include "sudoku.hpp"
#include <fstream>
#include <iostream>

// --- Construtores ---
Sudoku::Sudoku(const std::string& path) {
    if (!load_grid(path)) {
        std::cerr << "Falha ao carregar o tabuleiro. Gerando um novo com dificuldade média." << std::endl;
        generate_new_board(BoardGenerator::Difficulty::MEDIUM);
    }
}

Sudoku::Sudoku(BoardGenerator::Difficulty difficulty) {
    generate_new_board(difficulty);
}

Sudoku::Sudoku() {
    generate_new_board(BoardGenerator::Difficulty::MEDIUM);
}

// --- Métodos de Gerenciamento ---
void Sudoku::generate_new_board(BoardGenerator::Difficulty difficulty) {
    BoardGenerator generator;
    this->board = generator.generate(difficulty);
}


// --- Métodos de Validação (Otimizados) ---
bool Sudoku::check_line(int n) const {
    bool seen[10] = {false};
    for (int c = 0; c < 9; ++c) {
        int value = board.get_value(n, c);
        if (value != 0) {
            if (seen[value]) return false;
            seen[value] = true;
        }
    }
    return true;
}

bool Sudoku::check_column(int n) const {
    bool seen[10] = {false};
    for (int r = 0; r < 9; ++r) {
        int value = board.get_value(r, n);
        if (value != 0) {
            if (seen[value]) return false;
            seen[value] = true;
        }
    }
    return true;
}

bool Sudoku::check_box(int n) const {
    bool seen[10] = {false};
    int start_row = (n / 3) * 3;
    int start_col = (n % 3) * 3;
    for (int r_offset = 0; r_offset < 3; ++r_offset) {
        for (int c_offset = 0; c_offset < 3; ++c_offset) {
            int value = board.get_value(start_row + r_offset, start_col + c_offset);
            if (value != 0) {
                if (seen[value]) return false;
                seen[value] = true;
            }
        }
    }
    return true;
}

bool Sudoku::is_board_state_valid() const {
    for (int i = 0; i < 9; i++) {
        if (!check_line(i) || !check_column(i) || !check_box(i)) {
            return false;
        }
    }
    return true;
}

bool Sudoku::is_solved() const {
    return board.is_completed() && is_board_state_valid();
}

// --- Métodos de Interface ---
void Sudoku::print_grid() const {
    const std::string RED = "\033[31m";
    const std::string YELLOW = "\033[33m";
    const std::string RESET = "\033[0m";
    std::cout << "\n\n---------------------" << std::endl;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (c > 0 && c % 3 == 0) std::cout << "| ";
            int value = board.get_value(r, c);
            if (value == 0) {
                std::cout << "  ";
            } else {
                std::cout << (board.grid[r][c].is_fixed() ? RED : YELLOW) << value << RESET << " ";
            }
        }
        std::cout << std::endl;
        if ((r + 1) % 3 == 0 && r < 8) {
            std::cout << "---------------------" << std::endl;
        }
    }
    std::cout << "---------------------" << std::endl;
}

void Sudoku::start_game_loop() {
    while (true) {
        print_grid();
        auto move_data = get_move();
        int row = move_data.first.first;
        int col = move_data.first.second;
        int val = move_data.second;
        
        if (val == -1) {
             std::cout << "Saindo do jogo. Ate mais!" << std::endl;
             break;
        }

        if (!make_move(row, col, val)) {
            std::cout << "\nJogada invalida. Tente novamente.\n";
            continue;
        }

        if (is_solved()) {
            print_grid();
            std::cout << "\n\nParabens! Voce completou o Sudoku corretamente!\n\n";
            break;
        }
    }
}

std::pair<std::pair<int, int>, int> Sudoku::get_move() {
    int r, c, v;
    std::cout << "\nDigite a linha (0-8), coluna (0-8) e valor (1-9)." << std::endl;
    std::cout << "Para limpar uma celula, digite valor 0." << std::endl;
    std::cout << "Para sair, digite -1 em qualquer campo." << std::endl;
    std::cout << "Sua jogada (linha coluna valor): ";
    std::cin >> r >> c >> v;
    if (r == -1 || c == -1 || v == -1) return {{ -1, -1 }, -1 };
    return {{r, c}, v};
}

bool Sudoku::make_move(int x, int y, int value) {
    // se o valor for 0 nao faz a jogada
    if (value == 0) return false;

    int old_value = board.get_value(x, y);

    try {
        // faz a jogada
        board.change_value(x, y, value);

        // apaga as futuras jogadas se estiver "desfazendo" uma jogada
        if (current_move_index + 1 < (int)move_history.size()) {
            move_history.erase(move_history.begin() + current_move_index + 1, move_history.end());
        }

        // adiciona a jogada no histórico
        move_history.emplace_back(x, y, old_value, value);
        current_move_index++;

        return true;
    } catch (const std::runtime_error& e) {
        return false;
    }
}

// void Sudoku::add_move_to_log() {
//    (implementação futura)
// }