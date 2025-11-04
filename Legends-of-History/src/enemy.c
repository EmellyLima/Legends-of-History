#include "enemy.h"
#include "projectile.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <math.h>

// Emilly, porfavor, deixe comentários no seu código. Pleaseeeeee -w- valeu.
// -- Lucas

void enemy_init(Enemy *enemy, int start_x, int start_y, const char *sprite1, const char *sprite2) {
    enemy->x = start_x;
    enemy->y = start_y;
    enemy->move_timer = 0;
    enemy->fire_timer = 0;
    enemy->sprite = al_load_bitmap(sprite1);
    enemy->sprite2 = al_load_bitmap(sprite2);

    for (int i = 0; i < MAX_PROJECTILES; i++)
        projectile_init(&enemy->projectiles[i]);
}

void enemy_update(Enemy *enemy, Maze *maze) {
    enemy->move_timer++;
    if (enemy->move_timer > 30) {
        int dir = rand() % 4;
        int dx = 0, dy = 0;
        if (dir == 0) dy = -1;
        else if (dir == 1) dy = 1;
        else if (dir == 2) dx = -1;
        else dx = 1;

        int new_x = enemy->x + dx;
        int new_y = enemy->y + dy;

        if (maze_is_wall(maze, new_x, new_y)) {
            enemy->x = new_x;
            enemy->y = new_y;
        }
        enemy->move_timer = 0;
    }

    // 🔥 Disparo periódico
    enemy->fire_timer++;
    if (enemy->fire_timer > 90) {
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (!enemy->projectiles[i].active) {
                int dir = rand() % 4;
                projectile_fire(&enemy->projectiles[i], enemy->x, enemy->y, dir);
                break;
            }
        }
        enemy->fire_timer = 0;
    }

    for (int i = 0; i < MAX_PROJECTILES; i++)
        projectile_update(&enemy->projectiles[i], maze);
}

void enemy_draw(Enemy *enemy) {
    float scale = 1.0f; // 👈 reduz tamanho
    int w = al_get_bitmap_width(enemy->sprite);
    int h = al_get_bitmap_height(enemy->sprite);
    al_draw_scaled_bitmap(enemy->sprite, 0, 0, w, h,
                          enemy->x * TILE_SIZE, enemy->y * TILE_SIZE,
                          TILE_SIZE * scale, TILE_SIZE * scale, 0);

    for (int i = 0; i < MAX_PROJECTILES; i++)
        projectile_draw(&enemy->projectiles[i]);
}

void enemy_destroy(Enemy *enemy) {
    if (enemy->sprite) al_destroy_bitmap(enemy->sprite);
    if (enemy->sprite2) al_destroy_bitmap(enemy->sprite2);
}
