#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>

#include "config.h"
#include "menu.h"
#include "avatar_choice.h"

void menu_loop() {
    ALLEGRO_DISPLAY *display = al_get_current_display();
    if (!display) {
        fprintf(stderr, "Nenhum display ativo no menu.\n");
        return;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    ALLEGRO_BITMAP *logo = al_load_bitmap(LOGO_PATH);
    ALLEGRO_FONT *font = al_create_builtin_font();

    bool running = true;

    while (running) {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (logo) {
            int lw = al_get_bitmap_width(logo);
            int lh = al_get_bitmap_height(logo);
            al_draw_scaled_bitmap(logo, 0, 0, lw, lh,
                                  (SCREEN_WIDTH - lw) / 2, (SCREEN_HEIGHT - lh) / 4,
                                  lw, lh, 0);
        }

        al_draw_text(font, al_map_rgb(255, 255, 255),
                     SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.75,
                     ALLEGRO_ALIGN_CENTER, "Pressione ENTER para Jogar");
        al_draw_text(font, al_map_rgb(180, 180, 180),
                     SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.80,
                     ALLEGRO_ALIGN_CENTER, "Pressione ESC para Sair");

        al_flip_display();

        ALLEGRO_EVENT ev;
        if (al_wait_for_event_timed(queue, &ev, 0.05)) {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    running = false;
                    al_destroy_bitmap(logo);
                    al_destroy_font(font);
                    al_destroy_event_queue(queue);
                    avatar_choice_loop(); // chama escolha de avatar
                    return;
                } else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    running = false;
                }
            } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
        }
    }

    al_destroy_bitmap(logo);
    al_destroy_font(font);
    al_destroy_event_queue(queue);
}


