#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS 60

// 🧩 Labirinto
#define TILE_SIZE 48
#define MAZE_ROWS 25
#define MAZE_COLS 40

// Tipos de tile
#define T_FLOOR   0
#define T_WALL    1
#define T_PORTAL1 10
#define T_PORTAL2 11
#define T_PORTAL3 12
#define T_PORTAL4 13

// Centralização e mapeamento
#define MAZE_PIXEL_WIDTH   (MAZE_COLS * TILE_SIZE)
#define MAZE_PIXEL_HEIGHT  (MAZE_ROWS * TILE_SIZE)
#define MAZE_OFFSET_X      ((SCREEN_WIDTH - MAZE_PIXEL_WIDTH) / 2)
#define MAZE_OFFSET_Y      ((SCREEN_HEIGHT - MAZE_PIXEL_HEIGHT) / 2)
#define MAP_TO_SCREEN_X(x) ((x) * TILE_SIZE + MAZE_OFFSET_X)
#define MAP_TO_SCREEN_Y(y) ((y) * TILE_SIZE + MAZE_OFFSET_Y)

#define PLAYER_MALE_PATH   "assets/sprites/player_male.png"
#define PLAYER_FEMALE_PATH "assets/sprites/player_female.png"
#define LOGO_PATH "assets/sprites/logo.png"      // troque pra .jpeg se necessário
#define LABYRINTH_PATH "assets/sprites/labirinto.png"

#endif

