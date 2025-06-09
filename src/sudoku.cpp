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

bool Sudoku::load_grid(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;
    std::string grid_str;
    file >> grid_str;
    file.close();
    if (grid_str.length() != 162) return false;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int pos = 2 * (9 * r + c);
            int val = grid_str[pos] - '0';
            board.grid[r][c].set_value(val);
            if (grid_str[pos + 1] == 'f') {
                board.grid[r][c].fix();
            } else {
                board.grid[r][c].unfix();
            }
        }
    }
    return true;
}

std::string Sudoku::grid_to_string() const {
    std::string s = "";
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            s += std::to_string(board.grid[r][c].get_value());
            s += board.grid[r][c].is_fixed() ? 'f' : 'l';
        }
    }
    return s;
}

bool Sudoku::persist_grid(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    file << grid_to_string();
    file.close();
    return true;
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

bool Sudoku::make_move(int x, int y, int value){
    //se o valor for 0 a jogada nao sera feita
    if (value == 0) return false;

    // chama a função change_value de board que faz rodas as outras verificações na jogada e muda o valor caso seja valida
    return board.change_value(x, y, value);
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
        std::cerr << "Erro de parsing JSON ao carregar jogo de '" << filename << "': " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Erro inesperado ao carregar jogo de '" << filename << "': " << e.what() << std::endl;
        return false;
    }
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

const Board& Sudoku::get_board() const{
    return board;
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
