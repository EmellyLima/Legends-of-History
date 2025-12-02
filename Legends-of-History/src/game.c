#include "game.h"
#include "enemy.h"
#include "maze.h"
#include "player.h"
#include "config.h"
#include <allegro5/allegro_native_dialog.h>
#include <math.h>

void game_loop(const char *avatar) {
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    Maze maze;
    maze_load(&maze, 1);
    int player_x = 1;
    int player_y = 1;
    Player player;
    player_init(&player, player_x, player_y, avatar);

    Enemy enemies[MAX_ENEMIES]; // AVISO: EVITE EM COLOCAR O NÚMERO 5 NA COORD X --Lucas
    enemy_init(&enemies[0], 15, 7, "assets/sprites/knight_1.png", "assets/sprites/knight_1.png");
    enemy_init(&enemies[1], 10, 5, "assets/sprites/knight_2.png", "assets/sprites/knight_2.png");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool running = true;
    bool key[4] = {false, false, false, false};
    int current_level = 1;

    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                running = false;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) key[0] = true;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) key[1] = true;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) key[2] = true;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) key[3] = true;
        }

        if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP) key[0] = false;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) key[1] = false;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) key[2] = false;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) key[3] = false;
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            player_update(&player, &maze, key[0], key[1], key[2], key[3]);

            for (int i = 0; i < MAX_ENEMIES; i++)
                enemy_update(&enemies[i], &maze);

            // ⚔️ Colisão com inimigos
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if ((enemies[i].x - player_x) < 1.0f && (enemies[i].y - player_y) < 1.0f) {
                    al_show_native_message_box(display, "Derrota!", "Você foi atingido!", "Tente novamente!", NULL, ALLEGRO_MESSAGEBOX_WARN);
                    player_x = 1;
                    player_y = 1;
                }
            }

            // 🌀 Passar de fase
            int tile = maze.data[(int)player.y][(int)player.x];
            if (tile >= T_PORTAL1 && tile <= T_PORTAL4) {
                current_level++;
                if (current_level > 4) current_level = 1;
                maze_load(&maze, current_level);
                player.x = 1;
                player.y = 1;
            }

            al_clear_to_color(al_map_rgb(0, 0, 0));
            maze_draw(&maze);
            player_draw(&player);

            for (int i = 0; i < MAX_ENEMIES; i++)
                enemy_draw(&enemies[i]);

            al_flip_display();
        }
    }

    maze_unload(&maze);
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
}
