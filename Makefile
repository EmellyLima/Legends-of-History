SRC_DIR = src
INC_DIR = include
BIN_DIR = build

CC = gcc
CFLAGS = -Wall -I$(INC_DIR) `pkg-config --cflags allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5`
LDFLAGS = `pkg-config --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5`

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
TARGET = legends

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(TARGET)

run: all
	./$(TARGET)