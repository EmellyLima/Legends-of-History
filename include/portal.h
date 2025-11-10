#ifndef PORTAL_H
#define PORTAL_H

#include "player.h"
#include <stdbool.h>

/**
 * Estrutura que representa um portal no labirinto.
 * Cada portal tem uma posição (x, y) e uma área de colisão (width, height).
 */
typedef struct {
    float x, y;       // posição do portal no mundo
    float width;      // largura do portal
    float height;     // altura do portal
} Portal;

/**
 * Verifica se o jogador entrou em contato com o portal.
 *
 * @param player Ponteiro para o jogador.
 * @param portal Ponteiro para o portal.
 * @return true se houver colisão, false caso contrário.
 */
bool check_portal_collision(Player *player, Portal *portal);

#endif



