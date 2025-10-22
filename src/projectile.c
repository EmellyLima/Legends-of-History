#include "projectile.h"
#include "maze.h"
#include "config.h"
#include <allegro5/allegro_primitives.h>

void projectile_init(Projectile *p) {
    p->x = 0;
    p->y = 0;
    p->dx = 0;
    p->dy = 0;
    p->active = false;
}

void projectile_fire(Projectile *p, float start_x, float start_y, int direction) {
    p->x = start_x;
    p->y = start_y;
    p->active = true;

    switch (direction) {
        case 0: p->dx = 0; p->dy = -0.2f; break; // Cima
        case 1: p->dx = 0; p->dy = 0.2f; break;  // Baixo
        case 2: p->dx = -0.2f; p->dy = 0; break; // Esquerda
        case 3: p->dx = 0.2f; p->dy = 0; break;  // Direita
    }
}

void projectile_update(Projectile *p, Maze *maze) {
    if (!p->active) return;

    p->x += p->dx;
    p->y += p->dy;

    int gx = (int)p->x;
    int gy = (int)p->y;

    if (gx < 0 || gy < 0 || gx >= MAZE_COLS || gy >= MAZE_ROWS || maze_is_wall(maze, gx, gy)) {
        p->active = false;
    }
}

void projectile_draw(Projectile *p) {
    if (!p->active) return;

    al_draw_filled_circle(
        p->x * TILE_SIZE + TILE_SIZE / 2,
        p->y * TILE_SIZE + TILE_SIZE / 2,
        TILE_SIZE / 6,
        al_map_rgb(255, 60, 60)
    );
}

