// src/enemy.c
#include "enemy.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 
#include <stdio.h>

void init_enemy(Enemy *e, const char *sprite_path, float x, float y) {
    e->x = x;
    e->y = y;
    e->sprite = al_load_bitmap(sprite_path);
    if (!e->sprite) {
        fprintf(stderr, "Erro ao carregar sprite de inimigo: %s\n", sprite_path);
        e->width = e->height = 40;
    } else {
        e->width = al_get_bitmap_width(e->sprite);
        e->height = al_get_bitmap_height(e->sprite);
    }
    e->alive = true;
    e->hp = 1; 
}

void draw_enemy(Enemy *e) {
    if (!e->alive) return;
    if (e->sprite)
        al_draw_bitmap(e->sprite, e->x, e->y, 0);
    else
        al_draw_filled_rectangle(e->x, e->y, e->x + e->width, e->y + e->height, al_map_rgb(200,0,0));
}

bool enemy_collides_with_player(Enemy *e, float px, float py, int pw, int ph) {
    if (!e->alive) return false;
    return (px < e->x + e->width &&
            px + pw > e->x &&
            py < e->y + e->height &&
            py + ph > e->y);
}


