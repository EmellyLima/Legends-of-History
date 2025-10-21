CC = gcc
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
UNAME_S := $(shell uname -s)

CFLAGS = -Wall -g -I$(INCLUDE_DIR) -DALLEGRO_UNSTABLE -DALLEGRO_USE_CONSOLE
LDFLAGS = -lallegro -lallegro_main -lallegro_primitives -lallegro_image \
          -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

ifeq ($(UNAME_S), Darwin)
	CFLAGS += -I/opt/homebrew/include -I/opt/homebrew/opt/allegro/include
	LDFLAGS += -L/opt/homebrew/lib -L/opt/homebrew/opt/allegro/lib
	PLATFORM = macOS 
endif

ifeq ($(OS), Windows_NT)
	CFLAGS += -I"C:/allegro/include"
	LDFLAGS += -L"C:/allegro/lib"
	PLATFORM = Windows 
endif

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)
TARGET = $(BIN_DIR)/LegendsOfHistory
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	@echo "Linkando e gerando o executável para $(PLATFORM)..."
	$(CC) $(OBJ) -o $@ $(LDFLAGS)
	@echo "Compilação concluída com sucesso!"

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compilando $< ..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "🧹 Limpando arquivos de compilação..."
	rm -f $(OBJ) $(TARGET)
	@echo "Limpeza concluída!"

run: $(TARGET)
	@echo "Executando Legends of History..."
	./$(TARGET)





