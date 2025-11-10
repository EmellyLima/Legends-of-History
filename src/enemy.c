#include "enemy.h"
#include "maze.h"
#include "player.h"
#include "projectile.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>

static const float ENEMY_SIZE = 0.65f;

void enemy_init(Enemy *enemy, int start_x, int start_y, const char *sprite_path, EnemyType type) {
    if (!enemy) return;

    enemy->x = start_x;
    enemy->y = start_y;
    enemy->speed = 1.1f + ((float)(rand() % 20) / 100.0f); // leve variação de velocidade
    enemy->type = type;
    enemy->sprite = al_load_bitmap(sprite_path);

    if (!enemy->sprite) {
        printf("Erro ao carregar sprite do inimigo: %s\n", sprite_path);
    }

    // controla a frequência de tiro (para atiradores)
    enemy->cooldown = 0;
    enemy->fire_delay = 90.0f;  // frames entre tiros (~1.5s a 60 FPS)
    enemy->fire_timer = 0.0f;   // cronômetro interno

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        enemy->projectiles[i].active = false;
    }
}

void enemy_update(Enemy *enemy, Maze *maze, Player *player) {
    if (!enemy || !player) return;

    float dx = player->x - enemy->x;
    float dy = player->y - enemy->y;
    float dist = sqrtf(dx * dx + dy * dy);

    // Movimento básico de perseguição
    if (dist < TILE_SIZE * 10 && dist > TILE_SIZE * 0.5f) {
        float step_x = (dx / dist) * enemy->speed;
        float step_y = (dy / dist) * enemy->speed;

        int gx = (int)((enemy->x + step_x) / TILE_SIZE);
        int gy = (int)((enemy->y + step_y) / TILE_SIZE);

        if (!maze_is_wall(maze, gx, gy)) {
            enemy->x += step_x;
            enemy->y += step_y;
        }
    }

    // ATAQUE DOS ATIRADORES
    if (enemy->type == ENEMY_SHOOTER) {
        enemy->fire_timer += 1.0f; // incrementa o cronômetro a cada frame

        if (dist < TILE_SIZE * 8 && enemy->fire_timer >= enemy->fire_delay) {
            int dir;
            if (fabsf(dx) > fabsf(dy))
                dir = (dx > 0) ? 3 : 2; // direita/esquerda
            else
                dir = (dy > 0) ? 1 : 0; // baixo/cima

            for (int i = 0; i < MAX_PROJECTILES; i++) {
                if (!enemy->projectiles[i].active) {
                    float ox = enemy->x;
                    float oy = enemy->y;

                    switch (dir) {
                        case 0: oy -= TILE_SIZE * 0.3f; break;
                        case 1: oy += TILE_SIZE * 0.3f; break;
                        case 2: ox -= TILE_SIZE * 0.3f; break;
                        case 3: ox += TILE_SIZE * 0.3f; break;
                    }

                    projectile_fire(&enemy->projectiles[i], ox, oy, dir);
                    enemy->fire_timer = 0.0f; // reinicia o cronômetro de tiro
                    break;
                }
            }
        }
    }

    // ATUALIZA PROJÉTEIS
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (enemy->projectiles[i].active) {
            projectile_update(&enemy->projectiles[i]);
        }
    }
}

void enemy_draw(Enemy *enemy) {
    if (!enemy) return;

    float sx = MAZE_OFF_X + (enemy->x / TILE_SIZE) * MAZE_TILE_W;
    float sy = MAZE_OFF_Y + (enemy->y / TILE_SIZE) * MAZE_TILE_H;
    float w = MAZE_TILE_W * ENEMY_SIZE;
    float h = MAZE_TILE_H * ENEMY_SIZE;

    float dx = sx - w * 0.5f;
    float dy = sy - h * 0.5f;

    if (enemy->sprite) {
        al_draw_scaled_bitmap(
            enemy->sprite,
            0, 0,
            al_get_bitmap_width(enemy->sprite),
            al_get_bitmap_height(enemy->sprite),
            dx, dy, w, h, 0
        );
    } else {
        // fallback visual
        ALLEGRO_COLOR color = (enemy->type == ENEMY_SHOOTER)
                                ? al_map_rgb(255, 0, 0)
                                : al_map_rgb(255, 200, 50);
        al_draw_filled_rectangle(dx, dy, dx + w, dy + h, color);
    }

    // Desenha projéteis
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (enemy->projectiles[i].active)
            projectile_draw(&enemy->projectiles[i]);
    }
}
