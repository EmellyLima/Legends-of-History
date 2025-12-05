#include "enemy.h"
#include "maze.h"
#include "player.h"
#include "projectile.h"

#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const float ENEMY_SIZE = 0.65f;

/* Checa se célula é livre */
static inline bool cell_free(Maze *maze, int gx, int gy) {
    return !maze_is_wall(maze, gx, gy);
}

/* Inicialização */
void enemy_init(Enemy *enemy, int x, int y, const char *sprite_path, EnemyType type) {
    enemy->x = x;
    enemy->y = y;
    enemy->type = type;

    /* Velocidade base equilibrada */
    enemy->speed = 1.4f + ((float)(rand() % 30) / 100.0f);

    enemy->cooldown = 0;
    enemy->sprite = al_load_bitmap(sprite_path);

    if (!enemy->sprite)
        printf("Erro ao carregar sprite do inimigo: %s\n", sprite_path);

    for (int i = 0; i < MAX_PROJECTILES; i++)
        enemy->projectiles[i].active = false;
}

/* Movimento agressivo com navegação simples */
static void move_aggressively(Enemy *e, Maze *maze, Player *p) {
    float dx = p->x - e->x;
    float dy = p->y - e->y;
    float dist = sqrtf(dx*dx + dy*dy);

    if (dist < 1) dist = 1;

    float step = e->speed;

    float nx = e->x + (dx/dist)*step;
    float ny = e->y + (dy/dist)*step;

    int gx = nx / TILE_SIZE;
    int gy = ny / TILE_SIZE;

    if (cell_free(maze, gx, gy)) {
        e->x = nx;
        e->y = ny;
    } else {
        float ang = atan2f(dy, dx);
        for (int i = -3; i <= 3; i++) {
            float a = ang + (i * 0.4f);
            float tx = e->x + cosf(a)*step;
            float ty = e->y + sinf(a)*step;
            int gx2 = tx / TILE_SIZE;
            int gy2 = ty / TILE_SIZE;
            if (cell_free(maze, gx2, gy2)) {
                e->x = tx;
                e->y = ty;
                return;
            }
        }
    }
}

/* Atualização */
void enemy_update(Enemy *e, Maze *maze, Player *p, int level) {
    if (!e) return;

    /* Movimento */
    move_aggressively(e, maze, p);

    /* Distância ao player */
    float dx = p->x - e->x;
    float dy = p->y - e->y;
    float dist = sqrtf(dx*dx + dy*dy);

    /* Velocidade aumenta muito pouco por fase */
    e->speed = 1.4f + (level * 0.15f);

    /* ATIRADOR */
    if (e->type == ENEMY_SHOOTER) {
        if (e->cooldown > 0)
            e->cooldown--;

        float range = TILE_SIZE * 8.0f;

        int base = 90 - (level * 10);
        if (base < 45) base = 45;

        if (e->cooldown <= 0 && dist < range) {
            int dir;
            if (fabsf(dx) > fabsf(dy))
                dir = dx > 0 ? 3 : 2;
            else
                dir = dy > 0 ? 1 : 0;

            for (int i = 0; i < MAX_PROJECTILES; i++) {
                if (!e->projectiles[i].active) {
                    projectile_fire(&e->projectiles[i], e->x, e->y, dir);
                    e->cooldown = base;
                    break;
                }
            }
        }
    }

    /* Atualiza projéteis */
    for (int i = 0; i < MAX_PROJECTILES; i++)
        if (e->projectiles[i].active)
            projectile_update(&e->projectiles[i]);
}

/* Desenha */
void enemy_draw(Enemy *e) {
    if (!e) return;

    float sx = MAZE_OFF_X + (e->x / TILE_SIZE) * MAZE_TILE_W;
    float sy = MAZE_OFF_Y + (e->y / TILE_SIZE) * MAZE_TILE_H;
    float w = MAZE_TILE_W * ENEMY_SIZE;
    float h = MAZE_TILE_H * ENEMY_SIZE;

    if (e->sprite)
        al_draw_scaled_bitmap(
            e->sprite,
            0, 0,
            al_get_bitmap_width(e->sprite),
            al_get_bitmap_height(e->sprite),
            sx - w/2, sy - h/2, w, h, 0
        );
    else
        al_draw_filled_rectangle(
            sx - w/2, sy - h/2,
            sx + w/2, sy + h/2,
            e->type == ENEMY_SHOOTER ? al_map_rgb(255,0,0) :
                                       al_map_rgb(255,200,0)
        );

    for (int i = 0; i < MAX_PROJECTILES; i++)
        if (e->projectiles[i].active)
            projectile_draw(&e->projectiles[i]);
}
