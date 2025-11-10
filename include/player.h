#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include "projectile.h"
#include "maze.h"

/**
 * Estrutura que representa o jogador no jogo.
 */
typedef struct {
    float x, y;                     // posição atual no labirinto
    float speed;                    // velocidade de movimento
    int direction;                  // 0=cima, 1=baixo, 2=esq, 3=dir
    int lives;                      // número de vidas restantes
    ALLEGRO_BITMAP *sprite;         // imagem do personagem
    Projectile projectiles[MAX_PROJECTILES]; // tiros do jogador
} Player;

/**
 * Inicializa o jogador em uma posição inicial e carrega seu sprite.
 */
void player_init(Player *p, float start_x, float start_y, const char *sprite_path);

/**
 * Atualiza a posição e as ações do jogador com base nas teclas pressionadas.
 */
void player_update(Player *p, Maze *maze, ALLEGRO_KEYBOARD_STATE *key_state);

/**
 * Desenha o jogador e seus projéteis na tela.
 */
void player_draw(Player *p);

/**
 * Cria um novo projétil de acordo com a direção atual do jogador.
 */
void player_fire(Player *p);

/**
 * Trata colisões diretas entre o jogador e inimigos.
 */
void player_handle_collision(Player *p, float enemy_x, float enemy_y);

#endif










