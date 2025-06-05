const BACKEND_URL = 'http://localhost:8080';
const sudokuBoardElement = document.getElementById('sudoku-board');
const newGameButton = document.getElementById('newGameButton');
const messageElement = document.getElementById('message');

// Função para buscar um novo jogo do backend
async function fetchNewGame() {
    messageElement.textContent = "Carregando novo jogo...";
    try {
        const response = await fetch(`${BACKEND_URL}/api/new_game`, {
            method: 'POST', // Configurado como POST no main.cpp
            headers: {
                'Content-Type': 'application/json'
            }
        });

        if (!response.ok) { 
            const errorData = await response.json();
            throw new Error(`Erro HTTP: ${response.status} - ${errorData.message || 'Erro desconhecido do servidor'}`);
        }

        // Carrega os dados do jogo de .json para o javascript
        const data = await response.json(); 

        if (data.status === 'success') {
            //renderiza o tabuleiro
            renderBoard(data.board);
            messageElement.textContent = "Novo jogo carregado com sucesso!";
            setTimeout(() => messageElement.textContent = '', 3000);
        } else {
            messageElement.textContent = `Erro do servidor: ${data.message || 'Falha ao iniciar novo jogo.'}`;
        }

    } catch (error) {
        console.error('Erro ao buscar novo jogo:', error);
        messageElement.textContent = `Erro ao conectar com o servidor: ${error.message}. O servidor está rodando?`;
    }
}



// Função para renderizar o tabuleiro no HTML
function renderBoard(boardData) {
    // Limpa o tabuleiro existente
    sudokuBoardElement.innerHTML = ''; 
    boardData.grid.forEach((row, rowIndex) => {
        row.forEach((cellData, colIndex) => {
            const cellElement = document.createElement('div');
            cellElement.classList.add('cell');

            // Se o valor não for 0, exibe o número
            if (cellData.value !== 0) {
                cellElement.textContent = cellData.value;
            }

            // Adiciona a classe 'fixed' se for uma célula inicial do jogo
            if (cellData.is_fixed) {
                cellElement.classList.add('fixed');
            } else {
                cellElement.classList.add('unfixed'); 
            }
            
            cellElement.dataset.row = rowIndex;
            cellElement.dataset.col = colIndex;

            sudokuBoardElement.appendChild(cellElement);
        });
    });
}




// Adiciona um event listener para o botão "Novo Jogo"
newGameButton.addEventListener('click', fetchNewGame);

// Carrega um novo jogo automaticamente quando a página é carregada
document.addEventListener('DOMContentLoaded', fetchNewGame);