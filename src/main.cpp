#include "../include/httplib.hpp"
#include "../include/sudoku.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <string>

Sudoku game;

// FUNÇÃO AUXILIAR PARA APLICAR CABEÇALHOS CORS EM TODAS AS RESPOSTAS
void set_cors_headers(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

// Inicia um novo jogo e envia o tabuleiro inicial para o cliente.
void handle_new_game(const httplib::Request& req, httplib::Response& res) {
    set_cors_headers(res);

    game.new_game(1);

    nlohmann::json response_body;
    response_body["board"] = game.get_board();
    response_body["status"] = "success";

    res.set_content(response_body.dump(), "application/json");
    res.status = 200;
}

// carrega o tabuleiro salvo
void handle_load_game(const httplib::Request& req, httplib::Response& res) {
    // Adicione as configurações CORS 
    set_cors_headers(res);

    game.load_game("tests/grid.json"); 

    nlohmann::json response_body;
    response_body["board"] = game.get_board(); // Serializa o tabuleiro para JSON
    response_body["status"] = "success";

    // Define o conteúdo e tipo da resposta
    res.set_content(response_body.dump(), "application/json"); 
    res.status = 200; // 200 = OK
}

// faz um movimento se ele for valido
void handle_make_move(const httplib::Request& req, httplib::Response& res){
    set_cors_headers(res);

    int row, col, value;
    nlohmann::json request_body;

    // tenta ler o json do move
    try{
        request_body = nlohmann::json::parse(req.body);
        // Aceita tanto string quanto int para os campos
        if (request_body.contains("row")) request_body.at("row").get_to(row); else throw std::runtime_error("Campo 'row' ausente");
        if (request_body.contains("col")) request_body.at("col").get_to(col); else throw std::runtime_error("Campo 'col' ausente");
        if (request_body.contains("value")) request_body.at("value").get_to(value); else throw std::runtime_error("Campo 'value' ausente");
    }
    catch (const nlohmann::json::exception& e){
        std::cerr << "Erro ao parsear JSON em handle_make_move: " << e.what() << std::endl;
        nlohmann::json error_response;
        error_response["status"] = "error";
        error_response["message"] = "Formato de dados JSON invalido ou incompleto. Detalhe: " + std::string(e.what());
        res.set_content(error_response.dump(), "application/json");
        res.status = 400; // Bad Request
        return; 
    }
    catch (const std::exception& e){
        std::cerr << "Erro inesperado em handle_make_move: " << e.what() << std::endl;
        nlohmann::json error_response;
        error_response["status"] = "error";
        error_response["message"] = "Erro interno do servidor ao processar requisicao: " + std::string(e.what());
        res.set_content(error_response.dump(), "application/json");
        res.status = 500; // Internal Server Error
        return;
    }

    bool move_success = game.make_move(row, col, value);
    bool game_over = false;
    nlohmann::json response_body;
    response_body["board"] = game.get_board(); // Envia o tabuleiro atualizado 
    response_body["success"] = move_success; // Informa se a jogada foi válida
    response_body["game_over"] = false;
    response_body["status"] = move_success ? "success" : "error";

    if (move_success) {
        game_over = game.is_board_state_valid() && game.is_solved();
        response_body["game_over"] = game_over;
        if (game_over) {
            response_body["message"] = "Parabéns! Voce resolveu o Sudoku!";
        } else {
            response_body["message"] = "Jogada valida.";
        }
    } else {
        response_body["message"] = "Jogada invalida. Verifique regras do Sudoku ou celula pre-definida.";
    }

    res.set_content(response_body.dump(), "application/json"); 
    res.status = 200; // Sempre retorna 200 para facilitar o tratamento no frontend
}

int main(){
    // Cria uma instância do servidor HTTP
    httplib::Server svr;

    // cria a rota POST para new_game
    svr.Post("/api/load_game", handle_load_game);

    // cria a rota POST para new_game
    svr.Post("/api/new_game", handle_new_game);
    
    // cria a rota POST para make_move
    svr.Post("/api/make_move", handle_make_move);
    

    // localiza a pasta com a interface web do jogo
    svr.set_base_dir("./frontend");

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
