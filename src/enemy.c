#include "enemy.h"
#include "maze.h"
#include "player.h"
#include "projectile.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const float ENEMY_SIZE = 0.65f; // fator de escala do sprite

// Verifica se uma célula do labirinto está livre
static inline bool cell_free(Maze *maze, int gx, int gy) {
    return !maze_is_wall(maze, gx, gy);
}

/*
 * Inicializa o inimigo com suas propriedades básicas:
 * - posição inicial (x, y)
 * - tipo (atirador ou perseguidor)
 * - sprite (imagem correspondente)
 * - velocidade aleatória
 */
void enemy_init(Enemy *enemy, int start_x, int start_y, const char *sprite_path, EnemyType type) {
    if (!enemy) return;

    enemy->x = start_x;
    enemy->y = start_y;
    enemy->speed = 2.0f + ((float)(rand() % 50) / 100.0f); // variação entre 2.0 e 2.5
    enemy->cooldown = 0;
    enemy->type = type;
    enemy->sprite = al_load_bitmap(sprite_path);

    if (!enemy->sprite)
        printf("⚠️ Erro ao carregar sprite do inimigo: %s\n", sprite_path);

    for (int i = 0; i < MAX_PROJECTILES; i++)
        enemy->projectiles[i].active = false;
}

/*
 * Movimento agressivo: o inimigo tenta se mover até o jogador.
 * Se encontrar uma parede, testa ângulos alternativos para contornar obstáculos.
 */
static void move_aggressively(Enemy *enemy, Maze *maze, Player *player) {
    float dx = player->x - enemy->x;
    float dy = player->y - enemy->y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1) dist = 1;

    float step = enemy->speed * 1.4f;
    float nx = enemy->x + (dx / dist) * step;
    float ny = enemy->y + (dy / dist) * step;

    int gx = (int)(nx / TILE_SIZE);
    int gy = (int)(ny / TILE_SIZE);

    if (cell_free(maze, gx, gy)) {
        enemy->x = nx;
        enemy->y = ny;
    } else {
        float ang = atan2f(dy, dx);
        for (int i = -3; i <= 3; i++) {
            float a = ang + (i * 0.5f);
            float tx = enemy->x + cosf(a) * step;
            float ty = enemy->y + sinf(a) * step;
            gx = (int)(tx / TILE_SIZE);
            gy = (int)(ty / TILE_SIZE);
            if (cell_free(maze, gx, gy)) {
                enemy->x = tx;
                enemy->y = ty;
                break;
            }
        }
    }
}

/*
 * Atualiza o comportamento de um inimigo:
 * - movimentação
 * - ataques e tiros
 * - progressão de dificuldade
 */
void enemy_update(Enemy *enemy, Maze *maze, Player *player, int level) {
    if (!enemy || !maze || !player)
        return;

    // Movimento
    move_aggressively(enemy, maze, player);

    float dx = player->x - enemy->x;
    float dy = player->y - enemy->y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1) dist = 1;

    // Aumenta levemente a velocidade conforme a fase
    enemy->speed = 1.8f + (level * 0.2f);

    // --- ATIRADORES ---
    if (enemy->type == ENEMY_SHOOTER) {
        if (enemy->cooldown > 0)
            enemy->cooldown--;

        const float shoot_range = TILE_SIZE * 9.0f;

        // Dificuldade progressiva: quanto maior o nível, menor o intervalo entre tiros
        int base_delay = 150;
        int shoot_delay = base_delay - ((level - 1) * 30);
        if (shoot_delay < 45)
            shoot_delay = 45; // limite mínimo (~0.75s)

        // Detecção e disparo
        if (enemy->cooldown <= 0 && dist < shoot_range) {
            int dir;
            if (fabsf(dx) > fabsf(dy))
                dir = (dx > 0) ? 3 : 2;
            else
                dir = (dy > 0) ? 1 : 0;

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
                    enemy->cooldown = shoot_delay;
                    break;
                }
            }
        }
    }

    // --- PERSEGUIDORES ---
    if (enemy->type == ENEMY_CHASER && dist < TILE_SIZE * 0.7f) {
        player->lives -= 3;
        enemy->x -= dx / dist * TILE_SIZE * 0.5f;
        enemy->y -= dy / dist * TILE_SIZE * 0.5f;
    }

    // Atualiza projéteis ativos
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (enemy->projectiles[i].active)
            projectile_update(&enemy->projectiles[i]);
    }
}

/*
 * Desenha o inimigo e seus projéteis
 */
void enemy_draw(Enemy *enemy) {
    if (!enemy)
        return;

    float sx = MAZE_OFF_X + (enemy->x / TILE_SIZE) * MAZE_TILE_W;
    float sy = MAZE_OFF_Y + (enemy->y / TILE_SIZE) * MAZE_TILE_H;
    float w = MAZE_TILE_W * ENEMY_SIZE;
    float h = MAZE_TILE_H * ENEMY_SIZE;

    // Sprite ou fallback colorido
    if (enemy->sprite)
        al_draw_scaled_bitmap(enemy->sprite, 0, 0,
                              al_get_bitmap_width(enemy->sprite),
                              al_get_bitmap_height(enemy->sprite),
                              sx - w / 2, sy - h / 2, w, h, 0);
    else
        al_draw_filled_rectangle(
            sx - w / 2, sy - h / 2, sx + w / 2, sy + h / 2,
            (enemy->type == ENEMY_SHOOTER)
                ? al_map_rgb(255, 50, 50)
                : al_map_rgb(255, 200, 0));

    for (int i = 0; i < MAX_PROJECTILES; i++)
        if (enemy->projectiles[i].active)
            projectile_draw(&enemy->projectiles[i]);
}
