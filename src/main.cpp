#include "../include/httplib.hpp"
#include "../include/sudoku.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <string>

Sudoku game;

// Handler para a rota /api/new_game (POST)
// Inicia um novo jogo e envia o tabuleiro inicial para o cliente.
void handle_new_game(const httplib::Request& req, httplib::Response& res) {
    game.load_game("tests/grid.json"); 

    nlohmann::json response_body;
    response_body["board"] = game.get_board(); // Serializa o tabuleiro para JSON
    response_body["status"] = "success";

    res.set_content(response_body.dump(), "application/json"); // Define o conteúdo e tipo da resposta
    res.status = 200; // Define o status HTTP (200 OK)
}

int main(){
    // Cria uma instância do servidor HTTP
    httplib::Server svr;

    // cria a rota POST para new_game
    svr.Post("/api/new_game", handle_new_game);

    // localiza a pasta com a interface web do jogo
    svr.set_base_dir("../frontend");

    return 0;
}
