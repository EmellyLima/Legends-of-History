#include "enemy.h"
#include "maze.h"
#include "player.h"
#include "projectile.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const float ENEMY_SIZE = 0.65f; // fator de escala do sprite do inimigo

// Verifica se uma célula do labirinto está livre (sem parede)
static inline bool cell_free(Maze *maze, int gx, int gy) {
    return !maze_is_wall(maze, gx, gy);
}

/*
 * Inicializa o inimigo com suas propriedades básicas:
 * - posição inicial (x, y)
 * - tipo (atirador ou perseguidor)
 * - sprite (imagem correspondente)
 * - velocidade levemente aleatória
 */
void enemy_init(Enemy *enemy, int start_x, int start_y, const char *sprite_path, EnemyType type) {
    enemy->x = start_x;
    enemy->y = start_y;
    enemy->speed = 2.0f + ((float)(rand() % 50) / 100.0f); // variação entre 2.0 e 2.5
    enemy->cooldown = 0; // contador que define o tempo entre os tiros
    enemy->type = type;
    enemy->sprite = al_load_bitmap(sprite_path);

    // Desativa todos os projéteis no início
    for (int i = 0; i < MAX_PROJECTILES; i++)
        enemy->projectiles[i].active = false;
}

/*
 * Função de movimentação agressiva:
 * O inimigo tenta se mover diretamente até o jogador.
 * Caso encontre uma parede, ele testa ângulos alternativos (em torno da direção principal)
 * para contornar obstáculos — isso cria um comportamento mais “inteligente” e implacável.
 */
static void move_aggressively(Enemy *enemy, Maze *maze, Player *player) {
    float dx = player->x - enemy->x;
    float dy = player->y - enemy->y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1) dist = 1;

    // Velocidade ajustada para perseguição direta
    float step = enemy->speed * 1.4f;
    float nx = enemy->x + (dx / dist) * step;
    float ny = enemy->y + (dy / dist) * step;

    int gx = (int)(nx / TILE_SIZE);
    int gy = (int)(ny / TILE_SIZE);

    // Move na direção principal se o caminho estiver livre
    if (cell_free(maze, gx, gy)) {
        enemy->x = nx;
        enemy->y = ny;
    } else {
        // Se bloqueado, tenta ângulos laterais (busca alternativa)
        float ang = atan2f(dy, dx);
        for (int i = -3; i <= 3; i++) {
            float a = ang + (i * 0.5f); // muda o ângulo gradualmente
            float tx = enemy->x + cosf(a) * step;
            float ty = enemy->y + sinf(a) * step;
            gx = (int)(tx / TILE_SIZE);
            gy = (int)(ty / TILE_SIZE);
            if (cell_free(maze, gx, gy)) {
                enemy->x = tx;
                enemy->y = ty;
                break;
            }
        }
    }
}

/*
 * Atualiza o comportamento de um inimigo a cada frame:
 * - movimentação (chaser ou shooter)
 * - ataque (tiro ou contato)
 * - atualização dos projéteis
 */
void enemy_update(Enemy *enemy, Maze *maze, Player *player) {
    if (!enemy || !maze || !player) return;

    // Movimento constante em direção ao jogador
    move_aggressively(enemy, maze, player);

    float dx = player->x - enemy->x;
    float dy = player->y - enemy->y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1) dist = 1;

    /*
     * COMPORTAMENTO DO ATIRADOR (ENEMY_SHOOTER)
     * O inimigo detecta o jogador de qualquer distância e atira em intervalos curtos,
     * mas não constantes — agora há uma pausa maior entre disparos (~0.5s).
     */
    if (enemy->type == ENEMY_SHOOTER) {
        if (enemy->cooldown > 0)
            enemy->cooldown--;

        // Quando cooldown chega a zero, dispara um novo projétil
        if (enemy->cooldown <= 0) {
            int dir;
            // Calcula a direção predominante (horizontal ou vertical)
            if (fabsf(dx) > fabsf(dy))
                dir = (dx > 0) ? 3 : 2; // direita / esquerda
            else
                dir = (dy > 0) ? 1 : 0; // baixo / cima

            // Procura um projétil livre e dispara
            for (int i = 0; i < MAX_PROJECTILES; i++) {
                if (!enemy->projectiles[i].active) {
                    projectile_fire(&enemy->projectiles[i], enemy->x, enemy->y, dir);
                    break;
                }
            }

            // Recarrega o tempo até o próximo tiro (25–45 frames ≈ 0.5–0.75 segundos)
            enemy->cooldown = 25 + (rand() % 20);
        }

    } else {
        /*
         * COMPORTAMENTO DO PERSEGUIDOR (ENEMY_CHASER)
         * Se o inimigo encostar no jogador, causa dano pesado (–3 vidas)
         * e é levemente empurrado para trás para não ficar travado.
         */
        if (dist < TILE_SIZE * 0.7f) {
            player->lives -= 3;
            enemy->x -= dx / dist * TILE_SIZE * 0.5f;
            enemy->y -= dy / dist * TILE_SIZE * 0.5f;
        }
    }

    // Atualiza os projéteis disparados (movimento dos tiros ativos)
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (enemy->projectiles[i].active)
            projectile_update(&enemy->projectiles[i]);
    }
}

/*
 * Desenha o inimigo e seus tiros na tela.
 * A posição é convertida do sistema do labirinto (grid) para coordenadas de tela.
 * Se o sprite não estiver disponível, desenha um retângulo colorido de fallback.
 */
void enemy_draw(Enemy *enemy) {
    if (!enemy) return;

    float sx = MAZE_OFF_X + (enemy->x / TILE_SIZE) * MAZE_TILE_W;
    float sy = MAZE_OFF_Y + (enemy->y / TILE_SIZE) * MAZE_TILE_H;
    float w = MAZE_TILE_W * ENEMY_SIZE;
    float h = MAZE_TILE_H * ENEMY_SIZE;

    // Desenha o sprite ou um quadrado colorido se não houver imagem
    if (enemy->sprite)
        al_draw_scaled_bitmap(enemy->sprite, 0, 0,
                              al_get_bitmap_width(enemy->sprite),
                              al_get_bitmap_height(enemy->sprite),
                              sx - w / 2, sy - h / 2, w, h, 0);
    else
        al_draw_filled_rectangle(sx - w / 2, sy - h / 2, sx + w / 2, sy + h / 2,
                                 (enemy->type == ENEMY_SHOOTER)
                                     ? al_map_rgb(255, 50, 50)   // vermelho = atirador
                                     : al_map_rgb(255, 200, 0)); // amarelo = perseguidor

    // Desenha todos os projéteis ativos
    for (int i = 0; i < MAX_PROJECTILES; i++)
        if (enemy->projectiles[i].active)
            projectile_draw(&enemy->projectiles[i]);
}
