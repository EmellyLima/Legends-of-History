#include "intro.h"
#include "menu.h"
#include "config.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>
#include <stdio.h>

void intro_loop() {
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(5);
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Erro ao criar display da intro.\n");
        return;
    }

    ALLEGRO_BITMAP *logo = al_load_bitmap(LOGO_PATH);
    if (!logo) {
        fprintf(stderr, "Erro ao carregar logo: %s\n", LOGO_PATH);
    }

    ALLEGRO_SAMPLE *music = al_load_sample("assets/audio/menu_music.ogg");
    if (music) {
        al_play_sample(music, 0.8, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    }

    ALLEGRO_FONT *font_title = al_load_font("assets/fonts/font.ttf", 48, 0);
    ALLEGRO_FONT *font_sub = al_load_font("assets/fonts/font.ttf", 28, 0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    bool running = true;
    while (running) {
        al_clear_to_color(al_map_rgb(10, 10, 20));

        if (logo) {
            int lw = al_get_bitmap_width(logo);
            int lh = al_get_bitmap_height(logo);

            // Escala o logo para ocupar 50% da largura da tela
            float scale = (SCREEN_WIDTH * 0.5f) / lw;
            float scaled_w = lw * scale;
            float scaled_h = lh * scale;

            al_draw_scaled_bitmap(
                logo,
                0, 0, lw, lh,
                (SCREEN_WIDTH - scaled_w) / 2,
                (SCREEN_HEIGHT / 2) - scaled_h,
                scaled_w, scaled_h,
                0
            );
        }

        al_draw_text(font_title, al_map_rgb(255, 255, 255),
                     SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.7, ALLEGRO_ALIGN_CENTER,
                     "PRESSIONE ENTER PARA COMEÇAR");

        al_draw_text(font_sub, al_map_rgb(200, 200, 200),
                     SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.77, ALLEGRO_ALIGN_CENTER,
                     "PRESSIONE ESC PARA SAIR");

        al_flip_display();

        ALLEGRO_EVENT ev;
        if (al_wait_for_event_timed(queue, &ev, 0.05)) {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    running = false;
                    if (music) al_stop_samples();
                    al_destroy_bitmap(logo);
                    al_destroy_sample(music);
                    al_destroy_font(font_title);
                    al_destroy_font(font_sub);
                    al_destroy_event_queue(queue);
                    al_destroy_display(display);
                    menu_loop();
                    return;
                }
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    running = false;
                }
            } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
        }
    }

    al_destroy_bitmap(logo);
    al_destroy_sample(music);
    al_destroy_font(font_title);
    al_destroy_font(font_sub);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
}

