#include "projectile.h"
#include "maze.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>

extern float MAZE_TILE_W, MAZE_TILE_H, MAZE_OFF_X, MAZE_OFF_Y;

void projectile_init(Projectile *p, float start_x, float start_y, int direction) {
    if (!p) return;
    p->x = start_x;
    p->y = start_y;
    p->dir = direction;
    p->active = true;
}

void projectile_fire(Projectile *p, float start_x, float start_y, int direction) {
    projectile_init(p, start_x, start_y, direction);
}

void projectile_reset(Projectile *p) {
    if (!p) return;
    p->active = false;
    p->x = p->y = 0.0f;
    p->dir = 0;
}

void projectile_update(Projectile *p) {
    if (!p || !p->active) return;

    float step = PROJECTILE_SPEED;

    switch (p->dir) {
        case 0: p->y -= step; break;
        case 1: p->y += step; break;
        case 2: p->x -= step; break;
        case 3: p->x += step; break;
    }

    int gx = (int)(p->x / TILE_SIZE);
    int gy = (int)(p->y / TILE_SIZE);

    // Desativa se sair da área válida
    if (gx < 0 || gy < 0 || gx >= MAZE_COLS || gy >= MAZE_ROWS) {
        p->active = false;
        return;
    }

    // Desativa se sair da tela
    if (p->x < -TILE_SIZE * 2 || p->y < -TILE_SIZE * 2 ||
        p->x > (MAZE_COLS * TILE_SIZE + TILE_SIZE * 2) ||
        p->y > (MAZE_ROWS * TILE_SIZE + TILE_SIZE * 2)) {
        p->active = false;
    }
}

void projectile_draw(Projectile *p) {
    if (!p || !p->active) return;

    float sx = MAZE_OFF_X + (p->x / TILE_SIZE) * MAZE_TILE_W;
    float sy = MAZE_OFF_Y + (p->y / TILE_SIZE) * MAZE_TILE_H;

    float r = fmin(MAZE_TILE_W, MAZE_TILE_H) * 0.12f;
    al_draw_filled_circle(sx, sy, r, al_map_rgb(255, 230, 90));
}
