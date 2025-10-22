#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include "config.h"
#include "maze.h"
#include "projectile.h"

#define MAX_PROJECTILES 16
#define MAX_ENEMIES 2


typedef struct {
    int x, y;                 // Posição na grade (em tiles)
    int move_timer;
    int fire_timer;
    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *sprite2;
    Projectile projectiles[MAX_PROJECTILES];
} Enemy;

void enemy_init(Enemy *enemy, int start_x, int start_y,
                const char *sprite1, const char *sprite2);

void enemy_update(Enemy *enemy, Maze *maze);
void enemy_draw(Enemy *enemy);
void enemy_destroy(Enemy *enemy);

#endif




