const BACKEND_URL = 'http://localhost:8080';

// Elementos do HTML
const mainMenu = document.getElementById('mainMenu');
const loadGameButton = document.getElementById('loadGameButton');
const showNewGameOptionsButton = document.getElementById('showNewGameOptionsButton');
const newGameOptions = document.getElementById('newGameOptions');
const difficultySlider = document.getElementById('difficultySlider');
const startNewGameButton = document.getElementById('startNewGameButton');
const backToMenuButton = document.getElementById('backToMenuButton');
const sudokuBoardElement = document.getElementById('sudoku-board');
const messageElement = document.getElementById('message');

// Mapeamento de dificuldade para valores de slider
const difficulties = ['easy', 'medium', 'hard']; // Corresponde a min=0, max=2 do slider

// --- Funções de Exibição ---

function showMenu() {
    mainMenu.style.display = 'block';
    newGameOptions.style.display = 'none';
    sudokuBoardElement.style.display = 'none';
    messageElement.textContent = ''; // Limpa mensagens anteriores
}

function showNewGameDifficultyOptions() {
    mainMenu.style.display = 'none';
    newGameOptions.style.display = 'block';
    sudokuBoardElement.style.display = 'none';
    messageElement.textContent = '';
}

function showBoard() {
    mainMenu.style.display = 'none';
    newGameOptions.style.display = 'none';
    sudokuBoardElement.style.display = 'grid'; // 'grid' ou 'block' dependendo do seu CSS para o tabuleiro
    messageElement.textContent = '';
}

// --- Funções de Jogo ---

// Função para buscar um novo jogo do backend com dificuldade
async function fetchNewGame(difficulty) {
    messageElement.textContent = `Carregando novo jogo (${difficulty})...`;
    try {
        const response = await fetch(`${BACKEND_URL}/api/new_game`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
           // body: JSON.stringify({ difficulty: difficulty }) 
        });

        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(`Erro HTTP: ${response.status} - ${errorData.message || 'Erro desconhecido do servidor'}`);
        }

        const data = await response.json();

        if (data.status === 'success') {
            renderBoard(data.board);
            showBoard(); // Exibe o tabuleiro após carregado
            messageElement.textContent = `Novo jogo ${difficulty} carregado com sucesso!`;
            setTimeout(() => messageElement.textContent = '', 3000);
        } else {
            messageElement.textContent = `Erro do servidor: ${data.message || 'Falha ao iniciar novo jogo.'}`;
        }

    } catch (error) {
        console.error('Erro ao buscar novo jogo:', error);
        messageElement.textContent = `Erro ao conectar com o servidor: ${error.message}. O servidor está rodando?`;
    }
}

// Função para carregar um jogo salvo do backend
async function fetchLoadGame() {
    messageElement.textContent = "Carregando jogo salvo...";
    try {
        const response = await fetch(`${BACKEND_URL}/api/load_game`, {
            method: 'POST', 
            headers: {
                'Content-Type': 'application/json'
            }
        });

        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(`Erro HTTP: ${response.status} - ${errorData.message || 'Erro desconhecido ao carregar jogo'}`);
        }

        const data = await response.json();

        if (data.status === 'success') {
            renderBoard(data.board);
            showBoard(); // Exibe o tabuleiro após carregado
            messageElement.textContent = "Jogo salvo carregado com sucesso!";
            setTimeout(() => messageElement.textContent = '', 3000);
        } else {
            messageElement.textContent = `Erro do servidor: ${data.message || 'Nenhum jogo salvo encontrado ou falha ao carregar.'}`;
        }

    } catch (error) {
        console.error('Erro ao carregar jogo salvo:', error);
        messageElement.textContent = `Erro ao conectar com o servidor: ${error.message}. O servidor está rodando?`;
    }
}

// Função para renderizar o tabuleiro no HTML
function renderBoard(boardData) {
    sudokuBoardElement.innerHTML = '';
    boardData.grid.forEach((row, rowIndex) => {
        row.forEach((cellData, colIndex) => {
            const cellElement = document.createElement('div');
            cellElement.classList.add('cell');

            if (cellData.value !== 0) {
                cellElement.textContent = cellData.value;
            }

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

// --- Event Listeners ---

// Quando a página é carregada, mostra o menu principal
document.addEventListener('DOMContentLoaded', showMenu);

// Botão "Carregar Jogo Salvo"
loadGameButton.addEventListener('click', fetchLoadGame);

// Botão "Novo Jogo" (no menu principal)
showNewGameOptionsButton.addEventListener('click', showNewGameDifficultyOptions);

// Botão "Iniciar Novo Jogo" (nas opções de dificuldade)
startNewGameButton.addEventListener('click', () => {
    const selectedDifficultyIndex = difficultySlider.value;
    const selectedDifficulty = difficulties[selectedDifficultyIndex];
    fetchNewGame(selectedDifficulty);
});

// Botão "Voltar ao Menu"
backToMenuButton.addEventListener('click', showMenu);