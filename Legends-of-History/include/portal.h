#ifndef PORTAL_H
#define PORTAL_H

#include "player.h"
#include <stdbool.h>

typedef struct {
    float x, y;
    float width, height;
} Portal;

bool check_portal_collision(Player *player, Portal *portal);

#endif


