#include "game.h"
#include "maze.h"
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "quiz.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_ENEMIES 6
#define MAX_LIVES   5

static void spawn_enemies_for_level(Enemy enemies[MAX_ENEMIES], int level) {
    const char *chaser_sprite  = "assets/sprites/knight_1.png";
    const char *shooter_sprite = "assets/sprites/knight_2.png";

    int base_x[] = { 4, 10, 15, 20, 24,  7 };
    int base_y[] = { 5,  8, 10,  3, 14, 12 };

    float speed_boost = 1.0f + (level - 1) * 0.30f;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        const char *sprite = (i < 3) ? chaser_sprite : shooter_sprite;
        EnemyType type     = (i < 3) ? ENEMY_CHASER   : ENEMY_SHOOTER;
        enemy_init(&enemies[i],
                   TILE_SIZE * (base_x[i] + level),
                   TILE_SIZE * (base_y[i] + (i % 3)),
                   sprite, type);
        enemies[i].speed *= speed_boost;    // fases mais altas = inimigos mais rápidos
    }
}

void game_loop(const char *avatar_path, const char *player_name) {
    ALLEGRO_DISPLAY *display = al_get_current_display();
    if (!display) {
        printf("Erro: display não inicializado!\n");
        return;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    ALLEGRO_FONT *font = al_load_font("assets/fonts/font.ttf", 22, 0);
    if (!font) font = al_create_builtin_font();

    const char *sprite_path = "assets/sprites/player_male.png";
    if (avatar_path && strstr(avatar_path, "female"))
        sprite_path = "assets/sprites/player_female.png";

    Player player;
    player_init(&player, TILE_SIZE * 2, TILE_SIZE * 2, sprite_path);
    player.lives = 3;

    Maze maze;
    int current_level = 1;
    maze_load(&maze, current_level);

    Enemy enemies[MAX_ENEMIES];
    spawn_enemies_for_level(enemies, current_level);

    QuizBank qb;
    if (!load_quizzes(&qb, "assets/quizzes")) {
        printf("Aviso: Nenhum quiz encontrado. O jogo continuará sem perguntas.\n");
        qb.count = 0;
    }

    bool running = true;
    bool paused = false;
    bool bonus_dado = false;  // controla o +1 vida quando todos morrem
    ALLEGRO_EVENT ev;

    while (running) {
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_P)
                paused = !paused;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                running = false;
        }

        if (paused) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 120),
                         al_get_display_width(display)/2,
                         al_get_display_height(display)/2,
                         ALLEGRO_ALIGN_CENTER, "JOGO PAUSADO (P para continuar)");
            al_flip_display();
            continue;
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE key_state;
            al_get_keyboard_state(&key_state);
            player_update(&player, &maze, &key_state);

            // Inimigos: movimento e tiros
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].x >= 0)
                    enemy_update(&enemies[i], &maze, &player);
            }

            // Projéteis do jogador atingem inimigos
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].x < 0) continue;
                for (int j = 0; j < MAX_PROJECTILES; j++) {
                    Projectile *p = &player.projectiles[j];
                    if (!p->active) continue;
                    float dx = p->x - enemies[i].x;
                    float dy = p->y - enemies[i].y;
                    if (dx*dx + dy*dy < (TILE_SIZE * 0.50f) * (TILE_SIZE * 0.50f)) {
                        enemies[i].x = -9999;
                        enemies[i].y = -9999;
                        p->active = false;
                    }
                }
            }

            // Projéteis dos inimigos atingem o jogador (tira 1 vida)
            for (int i = 0; i < MAX_ENEMIES; i++) {
                for (int j = 0; j < MAX_PROJECTILES; j++) {
                    Projectile *p = &enemies[i].projectiles[j];
                    if (!p->active) continue;
                    float dx = p->x - player.x;
                    float dy = p->y - player.y;
                    if (dx*dx + dy*dy < (TILE_SIZE * 0.40f) * (TILE_SIZE * 0.40f)) {
                        p->active = false;
                        player.lives--;
                        if (player.lives <= 0) running = false;
                    }
                }
            }

            // Perseguidor encosta no jogador (tira 3 vidas de uma vez)
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].type == ENEMY_CHASER && enemies[i].x > 0) {
                    float dx = player.x - enemies[i].x;
                    float dy = player.y - enemies[i].y;
                    if (dx*dx + dy*dy < (TILE_SIZE * 0.60f) * (TILE_SIZE * 0.60f)) {
                        player.lives -= 3;
                        if (player.lives <= 0) running = false;
                    }
                }
            }

            // Portal encontrado -> quiz da fase
            int gx = (int)(player.x / TILE_SIZE);
            int gy = (int)(player.y / TILE_SIZE);
            if (gx >= 0 && gx < MAZE_COLS && gy >= 0 && gy < MAZE_ROWS) {
                int t = maze.data[gy][gx];
                if (t >= T_PORTAL1 && t <= T_PORTAL4) {
                    al_stop_timer(timer);
                    bool acertou = true;
                    if (qb.count > 0)
                        acertou = show_quiz_for_level(display, &qb, current_level);

                    if (!acertou)
                        current_level = (current_level > 1) ? current_level - 1 : 1;
                    else
                        current_level++;

                    if (current_level > 4) {
                        printf("PARABÉNS %s! Você completou todas as fases!\n", player_name);
                        running = false;
                    } else {
                        maze_unload(&maze);
                        maze_load(&maze, current_level);
                        spawn_enemies_for_level(enemies, current_level);
                        player.x = TILE_SIZE * 2;
                        player.y = TILE_SIZE * 2;
                        bonus_dado = false;   // reseta para poder ganhar vida novamente
                    }
                    al_start_timer(timer);
                }
            }

            // Se todos os 6 inimigos morreram -> +1 vida (até 5)
            bool todos_mortos = true;
            for (int i = 0; i < MAX_ENEMIES; i++)
                if (enemies[i].x > 0)
                    todos_mortos = false;

            if (todos_mortos && !bonus_dado) {
                if (player.lives < MAX_LIVES)
                    player.lives++;
                bonus_dado = true;
            }

            // Desenho
            al_clear_to_color(al_map_rgb(0, 0, 0));
            maze_draw(&maze);
            player_draw(&player);
            for (int i = 0; i < MAX_ENEMIES; i++)
                if (enemies[i].x > 0)
                    enemy_draw(&enemies[i]);

            al_draw_textf(font, al_map_rgb(255, 255, 255), 15, 15, 0,
                          "Vidas: %d | Fase: %d | Jogador: %s",
                          player.lives, current_level, player_name);
            al_flip_display();
        }
    }

    free_quizzes(&qb);
    maze_unload(&maze);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}
