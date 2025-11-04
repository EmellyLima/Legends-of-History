#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include "maze.h"

typedef struct {
    int grid_x;
    int grid_y;
    float x;
    float y;
    float speed;
    ALLEGRO_BITMAP *sprite;
} Player;

void player_init(Player *player, int start_x, int start_y, const char *sprite_path);
void player_update(Player *player, Maze *maze, int key_up, int key_down, int key_left, int key_right);
void player_draw(Player *player);
void player_destroy(Player *player);

#endif








