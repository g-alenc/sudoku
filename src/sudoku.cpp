#include "sudoku.hpp"
#include "BoardGenerator.hpp"
#include <fstream>
#include <iostream>

// --- Construtores ---
Sudoku::Sudoku(BoardGenerator::Difficulty difficulty) {
    generate_new_board(difficulty);
}

Sudoku::Sudoku() {
}

// --- Métodos de Gerenciamento ---
void Sudoku::generate_new_board(BoardGenerator::Difficulty difficulty) {
    BoardGenerator generator;
    this->board = generator.generate(difficulty);
}

const Board& Sudoku::get_board() const{
    return board;
}

// --- Métodos de Validação ---
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

bool Sudoku::is_board_state_valid() const{
    for (int i = 0; i < 9; i++) {
        if(!this->check_line(i) || !this->check_column(i) || !this->check_box(i)) {
            return false;
        }
    }
    return true;
}

bool Sudoku::is_completed(){
    return board.is_completed();
}

bool Sudoku::is_solved() const {
    return board.is_completed() && is_board_state_valid();
}

// --- Métodos de Manipulação de Estados ---
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

bool Sudoku::save_game(const string& filename) const {
    // Crie um objeto JSON vazio que representará o estado completo do jogo
    nlohmann::json j;

    // Serializar o tabuleiro (m_board)
    // Isso criará uma sub-estrutura JSON para o tabuleiro.
    j["board"] = board;

    // Abre o arquivo json ou o cria
    ofstream file(filename, ios::out | ios::trunc);

    // Verificar se o arquivo foi aberto com sucesso
    if (!file.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo para salvar o jogo: " << filename << endl;
        return false;
    }

    // Escrever o objeto JSON no arquivo
    // j.dump(4) converte o objeto JSON para uma string formatada com 4 espaços de indentação.
    file << j.dump(4);

    file.close();

    return true;
}

bool Sudoku::load_game(const string& filename){
    // carrega o arquivo
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Erro: Arquivo de jogo salvo '" << filename << "' nao encontrado ou nao pode ser aberto." << endl;
        return false;
    }

    // tenta ler e desserializar o JSON
    try {
        nlohmann::json j;
        // carrega o conteudo do json em j
        file >> j; 

        j.at("board").get_to(board);

        return true;

    } catch (const nlohmann::json::exception& e) {
        cerr << "Erro de parsing JSON ao carregar jogo de '" << filename << "': " << e.what() << endl;
        return false;
    } catch (const exception& e) {
        cerr << "Erro inesperado ao carregar jogo de '" << filename << "': " << e.what() << endl;
        return false;
    }
}

bool Sudoku::new_game(int diff){
    BoardGenerator bg;
    BoardGenerator::Difficulty difficulty;

    if (diff == 0) difficulty = BoardGenerator::Difficulty::EASY;
    else if (diff == 1) difficulty = BoardGenerator::Difficulty::MEDIUM;
    else if (diff == 2) difficulty = BoardGenerator::Difficulty::HARD;
    else if (diff == 3) difficulty = BoardGenerator::Difficulty::MASTER;
    else difficulty = BoardGenerator::Difficulty::MEDIUM;

    generate_new_board(difficulty);
    return true;
}

// --- Métodos de Interface (terminal) ---
void Sudoku::print_grid() const {
    const string RED = "\033[31m";
    const string YELLOW = "\033[33m";
    const string RESET = "\033[0m";
    cout << "\n\n---------------------" << endl;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (c > 0 && c % 3 == 0) cout << "| ";
            int value = board.get_value(r, c);
            if (value == 0) {
                cout << "  ";
            } else {
                cout << (board.grid[r][c].is_fixed() ? RED : YELLOW) << value << RESET << " ";
            }
        }
        cout << endl;
        if ((r + 1) % 3 == 0 && r < 8) {
            cout << "---------------------" << endl;
        }
    }
    cout << "---------------------" << endl;
}

void Sudoku::start_game_loop() {
    while (true) {
        print_grid();
        auto move_data = get_move();
        int row = move_data.first.first;
        int col = move_data.first.second;
        int val = move_data.second;
        
        if (val == -1) {
             cout << "Saindo do jogo. Ate mais!" << endl;
             break;
        }

        if (!make_move(row, col, val)) {
            cout << "\nJogada invalida. Tente novamente.\n";
            continue;
        }

        if (is_solved()) {
            print_grid();
            cout << "\n\nParabens! Voce completou o Sudoku corretamente!\n\n";
            break;
        }
    }
}

pair<pair<int, int>, int> Sudoku::get_move() {
    int r, c, v;
    cout << "\nDigite a linha (0-8), coluna (0-8) e valor (1-9)." << endl;
    cout << "Para limpar uma celula, digite valor 0." << endl;
    cout << "Para sair, digite -1 em qualquer campo." << endl;
    cout << "Sua jogada (linha coluna valor): ";
    cin >> r >> c >> v;
    if (r == -1 || c == -1 || v == -1) return {{ -1, -1 }, -1 };
    return {{r, c}, v};
}

// void Sudoku::add_move_to_log() {
//    (implementação futura)
// }
