# ==============================================================================
# variáveis de configuração
# ==============================================================================

CXX = g++

CXXFLAGS = -Wall -Iinclude -std=c++17

BUILD_DIR = build

# ==============================================================================
# configuração para o executável principal
# ==============================================================================

TARGET = $(BUILD_DIR)/my_program

SRCS = \
    src/board.cpp \
    src/BoardGenerator.cpp \
    src/cell.cpp \
    src/interface.cpp \
    src/main.cpp \
    src/sudoku.cpp

OBJS = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# ==============================================================================
# regras de compilação
# ==============================================================================

.PHONY: all
all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CXXFLAGS)

$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==============================================================================
# regras de execução
# ==============================================================================

.PHONY: run
run: $(TARGET)
	@echo "executando o programa principal: $(TARGET)"
	$(TARGET)

# ==============================================================================
# regra de limpeza
# ==============================================================================

.PHONY: clean
clean:
	@echo "limpando arquivos gerados..."
	rm -rf $(BUILD_DIR)