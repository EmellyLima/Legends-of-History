#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"
#include "player.h"
#include "enemies.h"
#include "maze.h"

int main() {
    if (!al_init()) {
        printf("Falha ao inicializar Allegro!\n");
        return -1;
    }

    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) {
        printf("Falha ao criar display!\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0); // 60 FPS

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    float player_x = 400;
    float player_y = 300;
    float speed = 5.0;

    bool running = true;
    bool redraw = true;
    ALLEGRO_EVENT ev;

    while (running) {
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } 
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        } 
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    player_y -= speed;
                    break;
                case ALLEGRO_KEY_DOWN:
                    player_y += speed;
                    break;
                case ALLEGRO_KEY_LEFT:
                    player_x -= speed;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    player_x += speed;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            // fundo preto
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // desenha player vermelho
            al_draw_filled_rectangle(player_x, player_y, player_x+50, player_y+50, al_map_rgb(255,0,0));

            al_flip_display();
        }
    }

    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
