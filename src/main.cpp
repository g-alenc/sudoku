#include "../include/httplib.hpp"
#include "../include/sudoku.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>

Sudoku game;


std::chrono::time_point<std::chrono::high_resolution_clock> game_start_time;


void set_cors_headers(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}


void handle_new_game(const httplib::Request& req, httplib::Response& res) {
    set_cors_headers(res);

    // Reinicia o contador de tempo do jogo
    game_start_time = std::chrono::high_resolution_clock::now();

    std::string difficulty_str = "medium";
    std::string game_name;
    
    if (!req.body.empty()) {
        try {
            auto body_json = nlohmann::json::parse(req.body);
            if (body_json.contains("difficulty")) {
                difficulty_str = body_json["difficulty"].get<std::string>();
            }
            if (body_json.contains("gameName")) {
                game_name = body_json["gameName"].get<std::string>();
            } else {
                nlohmann::json error_response;
                error_response["status"] = "error";
                error_response["message"] = "Nome do jogo não fornecido";
                res.set_content(error_response.dump(), "application/json");
                res.status = 400;
                return;
            }
        } catch (const std::exception& e) {
            nlohmann::json error_response;
            error_response["status"] = "error";
            error_response["message"] = std::string("Erro ao processar requisição: ") + e.what();
            res.set_content(error_response.dump(), "application/json");
            res.status = 400;
            return;
        }
    }

    int diff = 1;
    if (difficulty_str == "easy") diff = 0;
    else if (difficulty_str == "medium") diff = 1;
    else if (difficulty_str == "hard") diff = 2;
    else if (difficulty_str == "master") diff = 3;

    game.new_game(diff);
    game.board_name = game_name;

    nlohmann::json response_body;
    response_body["board"] = game.get_board();
    response_body["status"] = "success";

    res.set_content(response_body.dump(), "application/json");
    res.status = 200;
    std::cout << "Novo jogo iniciado. Tempo de jogo resetado." << std::endl;
}

// carrega o tabuleiro salvo
void handle_load_game(const httplib::Request& req, httplib::Response& res) {
    set_cors_headers(res);

    game_start_time = std::chrono::high_resolution_clock::now();

    game.load_game("tests/grid.json");
    std::string game_name;
    if (!req.body.empty()) {
        try {
            auto body_json = nlohmann::json::parse(req.body);
            if (body_json.contains("gameName")) {
                game_name = body_json["gameName"].get<std::string>();
            }
        } catch (const std::exception& e) {
            nlohmann::json error_response;
            error_response["status"] = "error";
            error_response["message"] = std::string("Erro ao processar requisição: ") + e.what();
            res.set_content(error_response.dump(), "application/json");
            res.status = 400;
            return;
        }
    }

    if (game_name.empty()) {
        nlohmann::json error_response;
        error_response["status"] = "error";
        error_response["message"] = "Nome do jogo não fornecido";
        res.set_content(error_response.dump(), "application/json");
        res.status = 400;
        return;
    }

    std::string load_path = "saves/" + game_name + ".json";
    bool load_success = game.load_game(load_path);

    if (!load_success) {
        nlohmann::json error_response;
        error_response["status"] = "error";
        error_response["message"] = "Erro ao carregar o jogo. Verifique se o arquivo existe.";
        res.set_content(error_response.dump(), "application/json");
        res.status = 404;
        return;
    }

    game.board_name = game_name;
    nlohmann::json response_body;
    response_body["board"] = game.get_board();
    response_body["status"] = "success";

    res.set_content(response_body.dump(), "application/json");
    res.status = 200; // 200 = OK
    std::cout << "Jogo carregado. Tempo de jogo resetado." << std::endl;
    res.status = 200;
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
            auto end_game_time = std::chrono::high_resolution_clock::now();
            auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(end_game_time - game_start_time); // Em segundos
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
    res.status = 200; // Sempre retorna 200 para facilitar o tratamento no frontend
}

// salva o jogo atual
void handle_save_game(const httplib::Request& req, httplib::Response& res) {
    set_cors_headers(res);

    std::string game_name;
    if (!req.body.empty()) {
        try {
            auto body_json = nlohmann::json::parse(req.body);
            if (body_json.contains("gameName")) {
                game_name = body_json["gameName"].get<std::string>();
            }
        } catch (const std::exception& e) {
            nlohmann::json error_response;
            error_response["status"] = "error";
            error_response["message"] = std::string("Erro ao processar requisição: ") + e.what();
            res.set_content(error_response.dump(), "application/json");
            res.status = 400;
            return;
        }
    }

    if (game_name.empty()) {
        game_name = game.board_name;
    }

    if (game_name.empty()) {
        nlohmann::json error_response;
        error_response["status"] = "error";
        error_response["message"] = "Nome do jogo não fornecido";
        res.set_content(error_response.dump(), "application/json");
        res.status = 400;
        return;
    }

    std::string save_path = "saves/" + game_name + ".json";
    bool save_success = game.save_game(save_path);

    nlohmann::json response_body;
    if (save_success) {
        response_body["status"] = "success";
        response_body["message"] = "Jogo salvo com sucesso!";
    } else {
        response_body["status"] = "error";
        response_body["message"] = "Erro ao salvar o jogo.";
    }

    res.set_content(response_body.dump(), "application/json");
    res.status = 200;
}

// lista todos os jogos salvos
void handle_list_games(const httplib::Request& req, httplib::Response& res) {
    set_cors_headers(res);

    nlohmann::json response_body;
    std::vector<std::string> saved_games;

    try {
        for (const auto& entry : std::filesystem::directory_iterator("saves")) {
            if (entry.path().extension() == ".json") {
                // Remove a extensão .json do nome do arquivo
                std::string game_name = entry.path().stem().string();
                saved_games.push_back(game_name);
            }
        }

        response_body["status"] = "success";
        response_body["games"] = saved_games;
        res.set_content(response_body.dump(), "application/json");
        res.status = 200;
    } catch (const std::exception& e) {
        response_body["status"] = "error";
        response_body["message"] = std::string("Erro ao listar jogos: ") + e.what();
        res.set_content(response_body.dump(), "application/json");
        res.status = 500;
    }
}

int main(){
    // Cria uma instância do servidor HTTP
    httplib::Server svr;

    // Cria o diretório saves se não existir
    std::filesystem::create_directory("saves");

    // cria a rota POST para new_game
    svr.Post("/api/load_game", handle_load_game);

    // cria a rota POST para new_game
    svr.Post("/api/new_game", handle_new_game);

    // cria a rota POST para make_move
    svr.Post("/api/make_move", handle_make_move);
    
    // cria a rota POST para save_game
    svr.Post("/api/save_game", handle_save_game);

    // cria a rota GET para listar jogos salvos
    svr.Get("/api/list_games", handle_list_games);

    // localiza a pasta com a interface web do jogo
    svr.set_base_dir("./frontend");

    // --- Start the Server ---
    std::cout << "C++ Sudoku Server listening on port 8080..." << std::endl;
    std::cout << "Access the web interface at: http://localhost:8080/index.html" << std::endl;
    std::cout << "Press Ctrl+C to stop the server." << std::endl;

    // O tempo de uptime do servidor geral continua aqui, se desejar
    // auto server_start_time = std::chrono::high_resolution_clock::now(); // Exemplo, se quiser o uptime total do servidor

    // The server will listen on all network interfaces (0.0.0.0) on port 8080.
    // If the port is in use or there's another error, listen() will return false.
    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "Error: Could not start the server on port 8080. The port might already be in use." << std::endl;
        return 1; // Return an error code
    }

    return 0;
}