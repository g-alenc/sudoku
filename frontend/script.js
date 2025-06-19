const BACKEND_URL = 'http://localhost:8080';

// Elementos do HTML
const mainMenu = document.getElementById('mainMenu');
const loadGameButton = document.getElementById('loadGameButton');
const showNewGameOptionsButton = document.getElementById('showNewGameOptionsButton');
const newGameOptions = document.getElementById('newGameOptions');
const difficultySlider = document.getElementById('difficultySlider');
const gameNameInput = document.getElementById('gameNameInput');
const startNewGameButton = document.getElementById('startNewGameButton');
const backToMenuButton = document.getElementById('backToMenuButton');
const sudokuBoardElement = document.getElementById('sudoku-board');
const gameControls = document.getElementById('gameControls');
const saveGameButton = document.getElementById('saveGameButton');
const backToMenuFromGameButton = document.getElementById('backToMenuFromGameButton');
const messageElement = document.getElementById('message');

// Mapeamento de dificuldade para valores de slider
const difficulties = ['easy', 'medium', 'hard', 'master']; // min=0, max=3

// --- Funções de Exibição ---

function showMenu() {
    mainMenu.style.display = 'block';
    newGameOptions.style.display = 'none';
    sudokuBoardElement.style.display = 'none';
    gameControls.style.display = 'none';
    messageElement.textContent = ''; // Limpa mensagens anteriores
    gameNameInput.value = ''; // Limpa o nome do jogo
}

function showNewGameDifficultyOptions() {
    mainMenu.style.display = 'none';
    newGameOptions.style.display = 'block';
    sudokuBoardElement.style.display = 'none';
    gameControls.style.display = 'none';
    messageElement.textContent = '';
}

function showBoard() {
    mainMenu.style.display = 'none';
    newGameOptions.style.display = 'none';
    sudokuBoardElement.style.display = 'grid'; // 'grid' ou 'block' dependendo do seu CSS para o tabuleiro
    gameControls.style.display = 'block';
    messageElement.textContent = '';
}

// --- Funções de Jogo ---

// Função para buscar um novo jogo do backend com dificuldade
async function fetchNewGame(difficulty) {
    const gameName = gameNameInput.value.trim();
    if (!gameName) {
        messageElement.textContent = 'Por favor, digite um nome para o jogo.';
        return;
    }

    messageElement.textContent = `Carregando novo jogo (${difficulty})...`;
    try {
        const response = await fetch(`${BACKEND_URL}/api/new_game`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ 
                difficulty,
                gameName
            })
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

// Função para salvar o jogo atual no backend
async function fetchSaveGame() {
    messageElement.textContent = "Salvando jogo...";
    try {
        const response = await fetch(`${BACKEND_URL}/api/save_game`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                gameName: gameNameInput.value.trim()
            })
        });

        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(`Erro HTTP: ${response.status} - ${errorData.message || 'Erro desconhecido ao salvar jogo'}`);
        }

        const data = await response.json();

        if (data.status === 'success') {
            messageElement.textContent = "Jogo salvo com sucesso!";
            setTimeout(() => messageElement.textContent = '', 3000);
        } else {
            messageElement.textContent = `Erro do servidor: ${data.message || 'Falha ao salvar jogo.'}`;
        }

    } catch (error) {
        console.error('Erro ao salvar jogo:', error);
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

            if (cellData.is_fixed) {
                if (cellData.value !== 0) {
                    cellElement.textContent = cellData.value;
                }
                cellElement.classList.add('fixed');
            } else {
                cellElement.classList.add('unfixed');
                if (cellData.value !== 0) {
                    cellElement.textContent = cellData.value;
                }
            }

            cellElement.dataset.row = rowIndex;
            cellElement.dataset.col = colIndex;

            sudokuBoardElement.appendChild(cellElement);
        });
    });
}

// Função para enviar uma jogada ao backend
async function fetchMakeMove(row, col, value) {
    try {
        const response = await fetch(`${BACKEND_URL}/api/make_move`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ row, col, value })
        });

        const data = await response.json();
        if (!response.ok) {
            throw new Error(data.message || 'Erro ao fazer jogada.');
        }
        renderBoard(data.board);
        if (data.success) {
            messageElement.textContent = data.message || 'Jogada válida!';
            if (data.game_over) {
                setTimeout(() => {
                    alert('Parabéns! Você resolveu o Sudoku!');
                    showMenu();
                }, 500);
            } else {
                setTimeout(() => messageElement.textContent = '', 2000);
            }
        } else {
            messageElement.textContent = data.message || 'Jogada inválida!';
            setTimeout(() => messageElement.textContent = '', 2000);
        }
    } catch (error) {
        console.error('Erro ao enviar jogada:', error);
        messageElement.textContent = `Erro ao conectar com o servidor: ${error.message}`;
    }
}

// Adiciona evento de clique nas células para permitir jogadas
function enableCellInput() {
    let selectedCell = null;
    let oldValue = '';

    sudokuBoardElement.addEventListener('click', function(event) {
        const cell = event.target;
        if (!cell.classList.contains('cell') || cell.classList.contains('fixed')) return;
        if (selectedCell) {
            selectedCell.classList.remove('editing');
        }
        selectedCell = cell;
        oldValue = cell.textContent.trim();
        cell.classList.add('editing');
    });

    document.addEventListener('keydown', async function(event) {
        if (!selectedCell) return;
        // Permite apenas números 0-9 e Esc
        if (event.key === 'Escape') {
            selectedCell.classList.remove('editing');
            selectedCell = null;
            return;
        }
        if (/^[0-9]$/.test(event.key)) {
            const value = parseInt(event.key);
            const row = parseInt(selectedCell.dataset.row);
            const col = parseInt(selectedCell.dataset.col);
            await fetchMakeMove(row, col, value);
            selectedCell.classList.remove('editing');
            selectedCell = null;
        }
    });
}

// --- Event Listeners ---

// Quando a página é carregada, mostra o menu principal
document.addEventListener('DOMContentLoaded', () => {
    showMenu();
    enableCellInput();
});

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

// Botão "Salvar Jogo"
saveGameButton.addEventListener('click', fetchSaveGame);

// Botão "Voltar ao Menu" (durante o jogo)
backToMenuFromGameButton.addEventListener('click', showMenu);