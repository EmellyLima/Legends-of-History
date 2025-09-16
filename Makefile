# Compilador
CC = clang

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Caminhos do vcpkg
VCPKG_INCLUDE = /Users/emellydapaixaolima/vcpkg/installed/x64-osx/include
VCPKG_LIB = /Users/emellydapaixaolima/vcpkg/installed/x64-osx/lib

# Flags
CFLAGS = -Wall -std=c99 -I$(VCPKG_INCLUDE)
LDFLAGS = -L$(VCPKG_LIB) -lallegro -lallegro_main -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image -lallegro_audio -lallegro_acodec

# Arquivos fonte e objetos
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
EXEC = $(BIN_DIR)/legends

# Regra principal
all: $(BIN_DIR) $(OBJ_DIR) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)