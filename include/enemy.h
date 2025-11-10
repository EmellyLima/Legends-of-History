#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <stdbool.h>
#include "projectile.h"
#include "maze.h"
#include "player.h"

typedef enum {
    ENEMY_CHASER,   // inimigo que persegue
    ENEMY_SHOOTER   // inimigo que atira
} EnemyType;

typedef struct {
    float x, y;
    float speed;
    int cooldown;
    EnemyType type;
    ALLEGRO_BITMAP *sprite;

    Projectile projectiles[MAX_PROJECTILES];
    float fire_delay;   // intervalo entre tiros
    float fire_timer;   // cronômetro interno de tiro
} Enemy;

void enemy_init(Enemy *enemy, int start_x, int start_y, const char *sprite_path, EnemyType type);
void enemy_update(Enemy *enemy, Maze *maze, Player *player);  // agora padronizado
void enemy_draw(Enemy *enemy);

#endif
