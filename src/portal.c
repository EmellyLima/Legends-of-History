#include "portal.h"
#include "player.h"
#include <math.h>
#include <stdbool.h>

/**
 * Verifica se o jogador colidiu com o portal.
 * A colisão é calculada com base na distância entre os centros.
 */
bool check_portal_collision(Player *player, Portal *portal) {
    if (!player || !portal) return false;

    float dx = player->x - portal->x;
    float dy = player->y - portal->y;
    float dist_sq = dx * dx + dy * dy;

    // Considera colisão se o jogador estiver próximo do centro do portal
    float radius = (portal->width + portal->height) * 0.25f;
    return dist_sq < (radius * radius);
}
