CC = clang
CFLAGS = -Wall -Iinclude $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5)
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5)

SRC = src
OBJ = obj
BIN = bin

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SOURCES))
EXEC = $(BIN)/LegendsofHistory

all: directories $(EXEC)

directories:
	mkdir -p $(OBJ) $(BIN)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)
