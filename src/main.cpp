#include "../include/httplib.hpp"
#include "../include/sudoku.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <string>

Sudoku game;

// Handler para a rota /api/new_game (POST)
// Inicia um novo jogo e envia o tabuleiro inicial para o cliente.
void handle_new_game(const httplib::Request& req, httplib::Response& res) {
    // --- Adicione as configurações CORS ---
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    
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


    // --- Start the Server ---
    std::cout << "C++ Sudoku Server listening on port 8080..." << std::endl;
    std::cout << "Access the web interface at: http://localhost:8080/index.html" << std::endl;
    std::cout << "Press Ctrl+C to stop the server." << std::endl;

    // The server will listen on all network interfaces (0.0.0.0) on port 8080.
    // If the port is in use or there's another error, listen() will return false.
    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "Error: Could not start the server on port 8080. The port might already be in use." << std::endl;
        return 1; // Return an error code
    }

    return 0;
}
