#include "player.h"
#include "maze.h"
#include "config.h"
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

void player_init(Player *player, int start_x, int start_y, const char *sprite_path) {
    player->grid_x = start_x;
    player->grid_y = start_y;
    player->sprite = al_load_bitmap(sprite_path);
    player->speed = PLAYER_SPEED;

    if (!player->sprite) {
        printf("Erro ao carregar sprite do jogador: %s\n", sprite_path);
    }
}

void player_update(Player *player, Maze *maze, int key_up, int key_down, int key_left, int key_right) {
    int new_x = player->grid_x;
    int new_y = player->grid_y;

    if (key_up)    new_y--;
    if (key_down)  new_y++;
    if (key_left)  new_x--;
    if (key_right) new_x++;

    if (new_x < 0 || new_x >= MAZE_COLS || new_y < 0 || new_y >= MAZE_ROWS)
        return;

    int tile = maze->data[new_y][new_x];

    if (tile == T_EMPTY || tile == T_FLOOR || (tile >= T_PORTAL1 && tile <= T_PORTAL4)) {
        player->grid_x = new_x;
        player->grid_y = new_y;
    }

    if (tile >= T_PORTAL1 && tile <= T_PORTAL4) {
        printf("Entrou no portal %d!\n", tile);
        maze->completed = true;
    }
}

void player_draw(Player *player) {
    float px = MAZE_OFF_X + player->grid_x * MAZE_TILE_W;
    float py = MAZE_OFF_Y + player->grid_y * MAZE_TILE_H;

    float tw = MAZE_TILE_W * 0.8f;  
    float th = MAZE_TILE_H * 0.8f;
    float dx = px + (MAZE_TILE_W - tw) / 2.0f;
    float dy = py + (MAZE_TILE_H - th) / 2.0f;

    if (player->sprite) {
        al_draw_scaled_bitmap(player->sprite,
                              0, 0,
                              al_get_bitmap_width(player->sprite),
                              al_get_bitmap_height(player->sprite),
                              dx, dy, tw, th, 0);
    } else {
        al_draw_filled_circle(
            dx + tw / 2, dy + th / 2,
            fminf(tw, th) / 2.5f,
            al_map_rgb(255, 0, 0)
        );
    }
}

void player_destroy(Player *player) {
    if (player->sprite) {
        al_destroy_bitmap(player->sprite);
        player->sprite = NULL;
    }
}

