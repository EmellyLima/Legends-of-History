#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdbool.h>
#include "maze.h"

typedef struct {
    float x, y;
    float dx, dy;
    bool active;
} Projectile;

void projectile_init(Projectile *p);
void projectile_fire(Projectile *p, float start_x, float start_y, int direction);
void projectile_update(Projectile *p, Maze *maze);
void projectile_draw(Projectile *p);

#endif

