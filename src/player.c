#include "player.h"
#include "config.h"
#include "maze.h"
#include "projectile.h"
#include "sound.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <math.h>
#include <stdio.h>

// Tamanho do sprite do jogador
static const float PLAYER_SIZE = 0.65f;

// Variáveis de som globais (definidas em sound.c)
extern bool g_audio_muted;
extern ALLEGRO_SAMPLE *g_snd_player_shoot;

void player_init(Player *p, float start_x, float start_y, const char *sprite_path) {
    p->x = start_x;
    p->y = start_y;
    p->speed = 1.8f;
    p->direction = 3;
    p->lives = 3;

    p->sprite = al_load_bitmap(sprite_path);
    if (!p->sprite) {
        printf("Erro ao carregar sprite do jogador: %s\n", sprite_path);
    }

    for (int i = 0; i < MAX_PROJECTILES; i++)
        p->projectiles[i].active = false;
}

void player_destroy(Player *p) {
    if (p && p->sprite) {
        al_destroy_bitmap(p->sprite);
        p->sprite = NULL;
    }
}

static bool can_move_to(float nx, float ny, Maze *maze) {
    int gx = (int)(nx / TILE_SIZE);
    int gy = (int)(ny / TILE_SIZE);
    return !maze_is_wall(maze, gx, gy);
}

void player_update(Player *p, Maze *maze, ALLEGRO_KEYBOARD_STATE *key_state) {
    if (!p || !maze) return;

    float nx = p->x;
    float ny = p->y;

    if (al_key_down(key_state, ALLEGRO_KEY_UP)) {
        ny -= p->speed;
        p->direction = 0;
    }
    if (al_key_down(key_state, ALLEGRO_KEY_DOWN)) {
        ny += p->speed;
        p->direction = 1;
    }
    if (al_key_down(key_state, ALLEGRO_KEY_LEFT)) {
        nx -= p->speed;
        p->direction = 2;
    }
    if (al_key_down(key_state, ALLEGRO_KEY_RIGHT)) {
        nx += p->speed;
        p->direction = 3;
    }

    if (can_move_to(nx, p->y, maze)) p->x = nx;
    if (can_move_to(p->x, ny, maze)) p->y = ny;

    // Disparo com ESPAÇO
    static bool space_prev = false;
    bool space_now = al_key_down(key_state, ALLEGRO_KEY_SPACE);
    if (space_now && !space_prev) {
        player_fire(p);
        if (!g_audio_muted) sound_play(g_snd_player_shoot);
    }
    space_prev = space_now;

    // Atualiza os projéteis ativos
    for (int i = 0; i < MAX_PROJECTILES; i++)
        if (p->projectiles[i].active)
            projectile_update(&p->projectiles[i]);
}

void player_draw(Player *p) {
    if (!p) return;

    float sx = MAZE_OFF_X + (p->x / TILE_SIZE) * MAZE_TILE_W;
    float sy = MAZE_OFF_Y + (p->y / TILE_SIZE) * MAZE_TILE_H;
    float w  = MAZE_TILE_W * PLAYER_SIZE;
    float h  = MAZE_TILE_H * PLAYER_SIZE;
    float dx = sx - w * 0.5f;
    float dy = sy - h * 0.5f;

    if (p->sprite) {
        al_draw_scaled_bitmap(
            p->sprite,
            0, 0,
            al_get_bitmap_width(p->sprite),
            al_get_bitmap_height(p->sprite),
            dx, dy, w, h,
            (p->direction == 2 ? ALLEGRO_FLIP_HORIZONTAL : 0)
        );
    } else {
        al_draw_filled_rectangle(dx, dy, dx + w, dy + h, al_map_rgb(0, 200, 255));
    }

    // Desenha projéteis ativos
    for (int i = 0; i < MAX_PROJECTILES; i++)
        if (p->projectiles[i].active)
            projectile_draw(&p->projectiles[i]);
}

void player_fire(Player *p) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!p->projectiles[i].active) {
            float ox = p->x, oy = p->y;
            switch (p->direction) {
                case 0: oy -= TILE_SIZE * 0.3f; break;
                case 1: oy += TILE_SIZE * 0.3f; break;
                case 2: ox -= TILE_SIZE * 0.3f; break;
                case 3: ox += TILE_SIZE * 0.3f; break;
            }
            projectile_fire(&p->projectiles[i], ox, oy, p->direction);
            break;
        }
    }
}

void player_handle_collision(Player *p, float enemy_x, float enemy_y) {
    float dx = p->x - enemy_x;
    float dy = p->y - enemy_y;
    float d2 = dx * dx + dy * dy;
    if (d2 < (TILE_SIZE * 0.45f) * (TILE_SIZE * 0.45f)) {
        p->lives--;
    }
}
