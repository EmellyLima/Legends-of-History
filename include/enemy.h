#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <stdbool.h>

typedef struct {
    float x, y;
    int width, height;
    ALLEGRO_BITMAP *sprite;
    bool alive;
    int hp;
} Enemy;

void init_enemy(Enemy *e, const char *sprite_path, float x, float y);
void draw_enemy(Enemy *e);
bool enemy_collides_with_player(Enemy *e, float px, float py, int pw, int ph);

#endif


