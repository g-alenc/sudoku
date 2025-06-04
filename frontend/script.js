const BACKEND_URL = 'http://localhost:8080';
const sudokuBoardElement = document.getElementById('sudoku-board');
const newGameButton = document.getElementById('newGameButton');
const messageElement = document.getElementById('message');

// Função para buscar um novo jogo (tabuleiro) do backend
async function fetchNewGame() {
    messageElement.textContent = "Carregando novo jogo...";
    try {
        const response = await fetch(`${BACKEND_URL}/api/new_game`, {
            method: 'POST', // Configurado como POST no seu main.cpp
            headers: {
                'Content-Type': 'application/json'
            }
        });

        if (!response.ok) { // Se o status HTTP não for 2xx (ex: 400, 404, 500)
            const errorData = await response.json(); // Tenta ler a mensagem de erro do servidor
            throw new Error(`Erro HTTP: ${response.status} - ${errorData.message || 'Erro desconhecido do servidor'}`);
        }

        const data = await response.json(); // Converte a resposta JSON em objeto JavaScript

        if (data.status === 'success') {
            renderBoard(data.board); // Chama a função para renderizar o tabuleiro na UI
            messageElement.textContent = "Novo jogo carregado com sucesso!";
            setTimeout(() => messageElement.textContent = '', 3000); // Limpa a mensagem após 3 segundos
        } else {
            // Caso o servidor retorne {status: "error"} mas com status HTTP 200
            messageElement.textContent = `Erro do servidor: ${data.message || 'Falha ao iniciar novo jogo.'}`;
        }

    } catch (error) {
        // Erro na requisição (ex: servidor não está rodando, problema de rede)
        console.error('Erro ao buscar novo jogo:', error);
        messageElement.textContent = `Erro ao conectar com o servidor: ${error.message}. O servidor está rodando?`;
    }
}

// Função para renderizar o tabuleiro no HTML
function renderBoard(boardData) {
    sudokuBoardElement.innerHTML = ''; // Limpa o tabuleiro existente
    boardData.grid.forEach((row, rowIndex) => {
        row.forEach((cellData, colIndex) => {
            const cellElement = document.createElement('div');
            cellElement.classList.add('cell');

            // Adiciona as classes para as bordas do Sudoku (CSS do style.css já faz muito)
            // Se o valor não for 0 (vazio), exibe o número
            if (cellData.value !== 0) {
                cellElement.textContent = cellData.value;
            }

            // Adiciona a classe 'predefined' se for uma célula inicial do puzzle
            if (cellData.is_predefined) {
                cellElement.classList.add('predefined');
            } else {
                cellElement.classList.add('editable'); // Células que o jogador pode preencher
            }
            
            // Atributos de dados para identificar a célula (linha e coluna) no JS
            cellElement.dataset.row = rowIndex;
            cellElement.dataset.col = colIndex;

            sudokuBoardElement.appendChild(cellElement);
        });
    });
}

// Adiciona um event listener para o botão "Novo Jogo"
newGameButton.addEventListener('click', fetchNewGame);

// Carrega um novo jogo automaticamente quando a página é carregada pela primeira vez
document.addEventListener('DOMContentLoaded', fetchNewGame);