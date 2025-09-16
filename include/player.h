#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>

typedef struct {
    float x, y;
    float speed;
    ALLEGRO_BITMAP *sprite;
} Player;

void init_player(Player *p);
void update_player(Player *p, ALLEGRO_KEYBOARD_STATE *key_state);
void draw_player(Player *p);

#endif