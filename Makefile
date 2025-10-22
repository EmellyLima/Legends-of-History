TARGET = bin/LegendsOfHistory
CC = gcc
INCLUDES = -Iinclude -I/opt/homebrew/include -I/opt/homebrew/opt/allegro/include
CFLAGS = -Wall -g -DALLEGRO_UNSTABLE -DALLEGRO_USE_CONSOLE
LIBS = -lallegro -lallegro_main -lallegro_primitives -lallegro_image \
       -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_dialog

# Diretórios de bibliotecas (para macOS / Linux)
LDFLAGS = -L/opt/homebrew/lib -L/opt/homebrew/opt/allegro/lib

OBJS = src/avatar_choice.o src/enemy.o src/game.o src/intro.o src/main.o \
       src/maze.o src/maze1.o src/maze2.o src/maze3.o src/maze4.o src/menu.o \
       src/player.o src/portal.o src/quiz.o src/sound.o src/projectile.o

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkando e gerando o executável..."
	$(CC) $(OBJS) -o $(TARGET) $(LIBS) $(LDFLAGS)

src/%.o: src/%.c
	@echo "Compilando $< ..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:
	@echo "🧹 Limpando arquivos compilados..."
	rm -f $(OBJS) $(TARGET)
run: all
	@echo " Executando o jogo..."
	./$(TARGET)
windows:
	@echo "Compilando para Windows..."
	$(CC) $(OBJS) -o $(TARGET).exe $(LIBS)
