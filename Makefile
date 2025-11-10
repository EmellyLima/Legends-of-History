# Makefile - Legends of History

CC = gcc
CFLAGS = -Wall -g -DALLEGRO_UNSTABLE -DALLEGRO_USE_CONSOLE \
	-Iinclude -I/opt/homebrew/include -I/opt/homebrew/opt/allegro/include

LDFLAGS = -L/opt/homebrew/lib -L/opt/homebrew/opt/allegro/lib \
	-lallegro -lallegro_main -lallegro_primitives -lallegro_image \
	-lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_dialog

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = $(SRC_DIR)

TARGET = $(BIN_DIR)/LegendsOfHistory

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linkando e gerando o executável..."
	@$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)
	@echo "Compilação concluída com sucesso."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compilando $< ..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Limpando arquivos temporários..."
	@rm -f $(SRC_DIR)/*.o
	@rm -f $(TARGET)
	@echo "Limpeza concluída."

run: all
	@echo "Executando Legends of History..."
	@./$(TARGET)

help:
	@echo "Comandos disponíveis:"
	@echo "  make           → Compila todo o projeto"
	@echo "  make run       → Compila e executa o jogo"
	@echo "  make clean     → Remove arquivos .o e o binário"
	@echo "  make help      → Mostra esta ajuda"
