#include "player.h"
#include "maze.h"
#include "config.h"
#include <allegro5/allegro_primitives.h>

void player_init(Player *p, int start_x, int start_y) {
    p->x = start_x;
    p->y = start_y;
}

void player_update(Player *p, int next_x, int next_y) {
    if (maze_is_free(next_x, next_y)) {
        p->x = next_x;
        p->y = next_y;
    }
}

void player_draw(Player *p) {
    al_draw_filled_rectangle(
        p->x * TAM, p->y * TAM,
        p->x * TAM + TAM, p->y * TAM + TAM,
        al_map_rgb(255, 255, 0) // Amarelo para o jogador
    );
}