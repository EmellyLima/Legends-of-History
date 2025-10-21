#include "game.h"
#include "player.h"
#include "maze.h"
#include "config.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdbool.h>

void start_game(int avatar) {
    // Inicializa Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro!\n");
        return;
    }

    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

    // Tela fullscreen
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    ALLEGRO_DISPLAY *display = al_create_display(1920, 1080);
    if (!display) {
        fprintf(stderr, "Falha ao criar display!\n");
        return;
    }

    int screen_w = al_get_display_width(display);
    int screen_h = al_get_display_height(display);

    // Eventos e timer
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // Fonte
    ALLEGRO_FONT *font = al_create_builtin_font();

    // Carrega maze inicial
    Maze maze;
    int level = 1;
    maze_load(&maze, level);

    // Player
    ALLEGRO_BITMAP *player_sprite = al_load_bitmap(
        avatar == 0 ? PLAYER_FEMALE_PATH : PLAYER_MALE_PATH);
    if (!player_sprite)
        fprintf(stderr, "Falha ao carregar sprite do jogador!\n");

    Player player = {
        .x = (MAZE_COLS * TILE_SIZE) / 2,
        .y = (MAZE_ROWS * TILE_SIZE) / 2,
        .speed = 4,
        .sprite = player_sprite
    };

    bool running = true;
    bool redraw = true;
    bool up = false, down = false, left = false, right = false;

    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_W: up = true; break;
                case ALLEGRO_KEY_S: down = true; break;
                case ALLEGRO_KEY_A: left = true; break;
                case ALLEGRO_KEY_D: right = true; break;
                case ALLEGRO_KEY_ESCAPE: running = false; break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_W: up = false; break;
                case ALLEGRO_KEY_S: down = false; break;
                case ALLEGRO_KEY_A: left = false; break;
                case ALLEGRO_KEY_D: right = false; break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Cálculo da próxima posição
            float new_x = player.x;
            float new_y = player.y;

            if (up) new_y -= player.speed;
            if (down) new_y += player.speed;
            if (left) new_x -= player.speed;
            if (right) new_x += player.speed;

            int grid_x = (int)(new_x / TILE_SIZE);
            int grid_y = (int)(new_y / TILE_SIZE);

            // Colisão
            if (!maze_is_wall(&maze, grid_y, grid_x)) {
                player.x = new_x;
                player.y = new_y;
            }

            // Detecta portal
            int tile = maze.data[grid_y][grid_x];
            if (tile >= T_PORTAL1 && tile <= T_PORTAL4) {
                level++;
                if (level > 4) {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_text(font, al_map_rgb(255, 255, 0),
                                 screen_w / 2, screen_h / 2,
                                 ALLEGRO_ALIGN_CENTER, "Parabéns! Você completou todas as fases!");
                    al_flip_display();
                    al_rest(3);
                    running = false;
                } else {
                    maze_unload(&maze);
                    maze_load(&maze, level);
                    player.x = (MAZE_COLS * TILE_SIZE) / 2;
                    player.y = (MAZE_ROWS * TILE_SIZE) / 2;
                }
            }

            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;

            al_clear_to_color(al_map_rgb(0, 0, 0));
            maze_draw(&maze);

            if (player.sprite)
                al_draw_scaled_bitmap(player.sprite, 0, 0,
                    al_get_bitmap_width(player.sprite),
                    al_get_bitmap_height(player.sprite),
                    player.x, player.y, TILE_SIZE, TILE_SIZE, 0);

            al_flip_display();
        }
    }

    // Libera tudo
    maze_unload(&maze);
    al_destroy_bitmap(player_sprite);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_font(font);
    al_destroy_display(display);
}
