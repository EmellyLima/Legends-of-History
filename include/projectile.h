#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "config.h"

// Número máximo de projéteis simultâneos
#ifndef MAX_PROJECTILES
#define MAX_PROJECTILES 10
#endif

// Velocidade padrão dos projéteis
#ifndef PROJECTILE_SPEED
#define PROJECTILE_SPEED 8.0f
#endif

/**
 * Estrutura que representa um projétil (bala de jogador ou inimigo).
 */
typedef struct {
    float x, y;     // posição em pixels
    int dir;        // direção (0=cima, 1=baixo, 2=esquerda, 3=direita)
    bool active;    // indica se o projétil está ativo
} Projectile;

/**
 * Inicializa um projétil inativo.
 */
void projectile_init(Projectile *p, float start_x, float start_y, int direction);

/**
 * Dispara um novo projétil a partir de uma posição e direção.
 */
void projectile_fire(Projectile *p, float start_x, float start_y, int direction);

/**
 * Atualiza a posição do projétil em movimento.
 */
void projectile_update(Projectile *p);

/**
 * Desenha o projétil na tela (pequeno círculo ou sprite).
 */
void projectile_draw(Projectile *p);

#endif



