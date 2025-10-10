#include "portal.h"
#include "config.h" // <<< ADICIONADO
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

static Portal portals[MAX_PORTALS];
static int portal_count = 0;

void portals_init() {
    portal_count = 3;
    
    portals[0] = (Portal){5, 1, 1};
    portals[1] = (Portal){18, 5, 1};
    portals[2] = (Portal){3, 9, 1};
}

void portals_draw() {
    for (int i = 0; i < portal_count; i++) {
        if (portals[i].active) {
            al_draw_filled_rectangle(
                portals[i].x * TAM, portals[i].y * TAM,
                portals[i].x * TAM + TAM, portals[i].y * TAM + TAM,
                al_map_rgb(50, 50, 255) // Azul
            );
        }
    }
}

int check_player_on_portal(int px, int py) {
    for (int i = 0; i < portal_count; i++) {
        if (portals[i].active && portals[i].x == px && portals[i].y == py) {
            return i;
        }
    }
    return -1;
}