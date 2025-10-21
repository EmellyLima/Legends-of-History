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
    // Inicializações Allegro
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(5);
    al_install_keyboard();

    // Cria janela
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Erro ao criar display da intro.\n");
        return;
    }

    // Carrega logo
    ALLEGRO_BITMAP *logo = al_load_bitmap(LOGO_PATH);
    if (!logo) {
        fprintf(stderr, "Erro ao carregar logo: %s\n", LOGO_PATH);
    }

    // Música
    ALLEGRO_SAMPLE *music = al_load_sample("assets/audio/menu_music.ogg");
    if (music) {
        al_play_sample(music, 0.8, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    }

    // Fonte
    ALLEGRO_FONT *font = al_load_font("assets/fonts/font.ttf", 36, 0);
    if (!font) {
        fprintf(stderr, "Fonte não encontrada!\n");
    }

    // Eventos
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    bool running = true;
    while (running) {
        al_clear_to_color(al_map_rgb(10, 10, 20)); 

        // Desenha logo centralizado e escalado
        if (logo) {
            float sw = al_get_display_width(display);
            float sh = al_get_display_height(display);
            float lw = al_get_bitmap_width(logo);
            float lh = al_get_bitmap_height(logo);

            float scale = 0.8f; 
            float logo_width = sw * scale;
            float logo_height = (logo_width / lw) * lh;
            float x = (sw - logo_width) / 2;
            float y = (sh - logo_height) / 3.5;

            al_draw_scaled_bitmap(logo, 0, 0, lw, lh, x, y, logo_width, logo_height, 0);
        }

        // Textos de instrução
        if (font) {
            al_draw_text(font, al_map_rgb(255, 255, 255),
                         SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.7,
                         ALLEGRO_ALIGN_CENTER, "PRESSIONE ENTER PARA COMEÇAR");
            al_draw_text(font, al_map_rgb(180, 180, 180),
                         SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.77,
                         ALLEGRO_ALIGN_CENTER, "PRESSIONE ESC PARA SAIR");
        }

        al_flip_display();
        ALLEGRO_EVENT ev;
        if (al_wait_for_event_timed(queue, &ev, 0.05)) {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    running = false;
                    if (music) al_stop_samples();
                    al_destroy_bitmap(logo);
                    al_destroy_sample(music);
                    al_destroy_font(font);
                    al_destroy_event_queue(queue);
                    al_destroy_display(display);
                    menu_loop(); 
                    return;
                } else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    running = false;
                }
            } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
        }
    }

    // Libera memória
    al_destroy_bitmap(logo);
    al_destroy_sample(music);
    al_destroy_font(font);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
}
