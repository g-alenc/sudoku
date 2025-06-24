#include "../include/httplib.hpp"
#include "../include/sudoku.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <string>
#include <chrono>

Sudoku game;

std::chrono::time_point<std::chrono::high_resolution_clock> game_start_time;

void set_cors_headers(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

void handle_new_game(const httplib::Request& req, httplib::Response& res) {
    set_cors_headers(res);

    game_start_time = std::chrono::high_resolution_clock::now();

    std::string difficulty_str = "medium";
    if (!req.body.empty()) {
        try {
            auto body_json = nlohmann::json::parse(req.body);
            if (body_json.contains("difficulty")) {
                difficulty_str = body_json["difficulty"].get<std::string>();
            }
        } catch (...) {}
    }

    int diff = 1;
    if (difficulty_str == "easy") diff = 0;
    else if (difficulty_str == "medium") diff = 1;
    else if (difficulty_str == "hard") diff = 2;
    else if (difficulty_str == "master") diff = 3;

    game.new_game(diff);

    nlohmann::json response_body;
    response_body["board"] = game.get_board();
    response_body["status"] = "success";

    res.set_content(response_body.dump(), "application/json");
    res.status = 200;
    std::cout << "Novo jogo iniciado. Tempo de jogo resetado." << std::endl;
}

void handle_load_game(const httplib::Request& req, httplib::Response& res) {
    set_cors_headers(res);

    game_start_time = std::chrono::high_resolution_clock::now();

    game.load_game("tests/grid.json");

    nlohmann::json response_body;
    response_body["board"] = game.get_board();
    response_body["status"] = "success";

    res.set_content(response_body.dump(), "application/json");
    res.status = 200;
    std::cout << "Jogo carregado. Tempo de jogo resetado." << std::endl;
}

void handle_make_move(const httplib::Request& req, httplib::Response& res){
    set_cors_headers(res);

    int row, col, value;
    nlohmann::json request_body;

    try{
        request_body = nlohmann::json::parse(req.body);
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
        res.status = 400;
        return;
    }
    catch (const std::exception& e){
        std::cerr << "Erro inesperado em handle_make_move: " << e.what() << std::endl;
        nlohmann::json error_response;
        error_response["status"] = "error";
        error_response["message"] = "Erro interno do servidor ao processar requisicao: " + std::string(e.what());
        res.set_content(error_response.dump(), "application/json");
        res.status = 500;
        return;
    }

    bool move_success = game.make_move(row, col, value);
    bool game_over = false;
    nlohmann::json response_body;
    response_body["board"] = game.get_board();
    response_body["success"] = move_success;
    response_body["game_over"] = false;
    response_body["status"] = move_success ? "success" : "error";

    if (move_success) {
        game_over = game.is_board_state_valid() && game.is_solved();
        response_body["game_over"] = game_over;
        if (game_over) {
            response_body["message"] = "Parabéns! Voce resolveu o Sudoku!";
            auto end_game_time = std::chrono::high_resolution_clock::now();
            auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(end_game_time - game_start_time);
            std::cout << "========================================" << std::endl;
            std::cout << "!!! JOGO DE SUDOKU ENCERRADO !!!" << std::endl;
            std::cout << "Parabéns! Você resolveu o Sudoku em: " << game_duration.count() << " segundos." << std::endl;
            std::cout << "========================================" << std::endl;
        } else {
            response_body["message"] = "Jogada valida.";
        }
    } else {
        response_body["message"] = "Jogada invalida. Verifique regras do Sudoku ou celula pre-definida.";
    }

    res.set_content(response_body.dump(), "application/json");
    res.status = 200;
}

int main(){
    httplib::Server svr;

    svr.Post("/api/load_game", handle_load_game);

    svr.Post("/api/new_game", handle_new_game);

    svr.Post("/api/make_move", handle_make_move);

    svr.set_base_dir("./frontend");

    std::cout << "C++ Sudoku Server listening on port 8080..." << std::endl;
    std::cout << "Access the web interface at: http://localhost:8080/index.html" << std::endl;
    std::cout << "Press Ctrl+C to stop the server." << std::endl;

    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "Error: Could not start the server on port 8080. The port might already be in use." << std::endl;
        return 1;
    }

    return 0;
}