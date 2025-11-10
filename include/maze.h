#ifndef MAZE_H
#define MAZE_H

#include <allegro5/allegro.h>
#include <stdbool.h>
#include "config.h"

/**
 * Estrutura principal que representa um labirinto.
 */
typedef struct {
    int data[MAZE_ROWS][MAZE_COLS];     // mapa lógico (paredes, chão, portais)
    ALLEGRO_BITMAP *wall_img;           // textura das paredes
    ALLEGRO_BITMAP *floor_img;          // textura do chão
    ALLEGRO_BITMAP *portal_img[4];      // texturas dos portais
    bool completed;                     // controle de conclusão da fase
} Maze;

// Variáveis globais de escala e posicionamento
extern float MAZE_TILE_W;
extern float MAZE_TILE_H;
extern float MAZE_OFF_X;
extern float MAZE_OFF_Y;

// Funções principais do labirinto
void maze_load(Maze *maze, int level);
void maze_draw(Maze *maze);
void maze_unload(Maze *maze);
bool maze_is_wall(Maze *maze, int grid_x, int grid_y);

// Funções de carregamento de fases
void maze1_load(int data[MAZE_ROWS][MAZE_COLS]);
void maze2_load(int data[MAZE_ROWS][MAZE_COLS]);
void maze3_load(int data[MAZE_ROWS][MAZE_COLS]);
void maze4_load(int data[MAZE_ROWS][MAZE_COLS]);

// Funções auxiliares de detecção de tiles e portais
/**
 * Retorna o tipo de tile em uma coordenada do mundo.
 * Exemplo: T_WALL, T_PORTAL1, etc.
 */
int maze_tile_at(Maze *maze, float world_x, float world_y);

/**
 * Retorna true se há um portal na posição especificada.
 * Se `out_idx` não for NULL, ele receberá o índice (0–3) do portal.
 */
bool maze_is_portal_here(Maze *maze, float world_x, float world_y, int *out_idx);

#endif





