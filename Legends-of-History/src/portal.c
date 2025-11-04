#include "portal.h"
#include <allegro5/allegro.h>

bool check_portal_collision(Player *player, Portal *portal) {
    float pw = al_get_bitmap_width(player->sprite);
    float ph = al_get_bitmap_height(player->sprite);

    return player->x + pw > portal->x &&
           player->x < portal->x + portal->width &&
           player->y + ph > portal->y &&
           player->y < portal->y + portal->height;
}

