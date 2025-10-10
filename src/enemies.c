#include "enemies.h"
#include "maze.h"
#include "config.h" // <<< ADICIONADO
#include <allegro5/allegro_primitives.h>

static Enemy enemies[MAX_ENEMIES];
static int enemy_count = 0;

void enemies_init() {
    enemy_count = 3;
    enemies[0] = (Enemy){8, 1, 0};
    enemies[1] = (Enemy){10, 5, 1};
    enemies[2] = (Enemy){15, 9, 2};
}

int check_player_collision(int px, int py) {
    for (int i = 0; i < enemy_count; i++) {
        if (enemies[i].x == px && enemies[i].y == py) return 1;
    }
    return 0;
}

void enemies_update() {
    for (int i = 0; i < enemy_count; i++) {
        int nx = enemies[i].x;
        int ny = enemies[i].y;
        switch (enemies[i].dir) { 
            case 0: nx++; break; // Direita
            case 1: ny++; break; // Baixo
            case 2: nx--; break; // Esquerda
            case 3: ny--; break; // Cima
        }
        if (maze_is_free(nx, ny)) {
            enemies[i].x = nx;
            enemies[i].y = ny;
        } else {
            enemies[i].dir = (enemies[i].dir + 1) % 4;
        }
    }
}

void enemies_draw() {
    for (int i = 0; i < enemy_count; i++) {
        al_draw_filled_rectangle(
            enemies[i].x * TAM, enemies[i].y * TAM,
            enemies[i].x * TAM + TAM, enemies[i].y * TAM + TAM,
            al_map_rgb(200, 30, 30)
        );
    }
}