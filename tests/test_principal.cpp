#include <iostream>
#include <limits> // Necessário para limpar o buffer de entrada (cin)
#include "sudoku.hpp"

// Função para exibir o menu principal
void exibir_menu() {
    std::cout << "\n===== JOGO DE SUDOKU =====\n";
    std::cout << "1. Gerar Novo Jogo\n";
    std::cout << "2. Carregar Jogo de Arquivo\n";
    std::cout << "3. Sair\n";
    std::cout << "Escolha uma opcao: ";
}

// Função para exibir o menu de dificuldade
BoardGenerator::Difficulty escolher_dificuldade() {
    int escolha_diff = 2; // Padrão é Médio
    std::cout << "\n--- Escolha a Dificuldade ---\n";
    std::cout << "1. Facil\n";
    std::cout << "2. Medio\n";
    std::cout << "3. Dificil\n";
    std::cout << "4. Mestre\n";
    std::cout << "Escolha uma dificuldade: ";
    std::cin >> escolha_diff;

    switch (escolha_diff) {
        case 1: return BoardGenerator::Difficulty::EASY;
        case 3: return BoardGenerator::Difficulty::HARD;
        case 4: return BoardGenerator::Difficulty::MASTER;
        case 2:
        default: return BoardGenerator::Difficulty::MEDIUM;
    }
}

int main() {
    int escolha_menu;

    while (true) {
        exibir_menu();
        std::cin >> escolha_menu;

        // Limpa o buffer de entrada caso o usuário digite algo que não seja um número
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            escolha_menu = 0; // Força uma opção inválida
        }

        if (escolha_menu == 1) {
            // Gerar Novo Jogo
            BoardGenerator::Difficulty diff = escolher_dificuldade();
            Sudoku jogo(diff); // Usa o construtor que gera um novo tabuleiro
            jogo.start_game_loop();

        } else if (escolha_menu == 2) {
            // Carregar Jogo de Arquivo
            std::string path;
            std::cout << "Digite o caminho do arquivo para carregar: ";
            std::cin >> path;
            Sudoku jogo(path); // Usa o construtor que carrega de um arquivo
            jogo.start_game_loop();

        } else if (escolha_menu == 3) {
            // Sair
            std::cout << "Obrigado por jogar! Ate mais.\n";
            break;
        } else {
            std::cout << "Opcao invalida. Por favor, tente novamente.\n";
        }
    }

    return 0;
}