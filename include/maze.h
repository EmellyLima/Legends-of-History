#ifndef MAZE_H
#define MAZE_H

#include <allegro5/allegro.h>
#include <stdbool.h>
#include "config.h"

typedef struct {
    int data[MAZE_ROWS][MAZE_COLS];
    ALLEGRO_BITMAP *wall_img;
    ALLEGRO_BITMAP *floor_img;
    ALLEGRO_BITMAP *portal_img[4];
    bool completed;
} Maze;

// Variáveis globais para centralização e escala
extern float MAZE_TILE_W;
extern float MAZE_TILE_H;
extern float MAZE_OFF_X;
extern float MAZE_OFF_Y;

void maze_load(Maze *maze, int level);
void maze_draw(Maze *maze);
void maze_unload(Maze *maze);
bool maze_is_wall(Maze *maze, int row, int col);

void maze1_load(int data[MAZE_ROWS][MAZE_COLS]);
void maze2_load(int data[MAZE_ROWS][MAZE_COLS]);
void maze3_load(int data[MAZE_ROWS][MAZE_COLS]);
void maze4_load(int data[MAZE_ROWS][MAZE_COLS]);

#endif






