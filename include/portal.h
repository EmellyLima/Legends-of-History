#ifndef PORTAL_H
#define PORTAL_H

#define MAX_PORTALS 5

typedef struct {
    int x;
    int y;
    int active;
} Portal;

void portals_init();
void portals_draw();
int check_player_on_portal(int px, int py);

#endif