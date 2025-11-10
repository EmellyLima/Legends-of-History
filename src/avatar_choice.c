#include "avatar_choice.h"
#include "config.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <string.h>

bool avatar_choice_loop(ALLEGRO_DISPLAY *display, char *out_path, int out_cap) {
    if (!display) {
        fprintf(stderr, "avatar_choice_loop: display é NULL\n");
        return false;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Erro ao criar fila de eventos.\n");
        return false;
    }

    ALLEGRO_FONT *font = al_load_font("assets/fonts/font.ttf", 28, 0);
    if (!font) font = al_create_builtin_font();

    ALLEGRO_BITMAP *male   = al_load_bitmap(PLAYER_MALE_PATH);
    ALLEGRO_BITMAP *female = al_load_bitmap(PLAYER_FEMALE_PATH);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    int selected = 0; // 0 = masculino, 1 = feminino
    bool running = true, ok = false;

    int screen_w = al_get_display_width(display);
    int screen_h = al_get_display_height(display);

    while (running) {
        al_clear_to_color(al_map_rgb(10, 10, 15));
        al_draw_text(font, al_map_rgb(230, 230, 230),
                     screen_w / 2, 60, ALLEGRO_ALIGN_CENTER,
                     "ESCOLHA SEU PERSONAGEM (◀ ▶ e ENTER)");

        float scale = 0.30f;
        int spacing = 200;

        if (male) {
            int mw = al_get_bitmap_width(male), mh = al_get_bitmap_height(male);
            al_draw_scaled_bitmap(male, 0, 0, mw, mh,
                screen_w / 2 - mw * scale - spacing / 2, screen_h / 2 - mh * scale / 2,
                mw * scale, mh * scale, 0);
        }

        if (female) {
            int fw = al_get_bitmap_width(female), fh = al_get_bitmap_height(female);
            al_draw_scaled_bitmap(female, 0, 0, fw, fh,
                screen_w / 2 + spacing / 2, screen_h / 2 - fh * scale / 2,
                fw * scale, fh * scale, 0);
        }

        const char *label = (selected == 0) ? "MASCULINO" : "FEMININO";
        ALLEGRO_COLOR color = (selected == 0)
                                ? al_map_rgb(100, 220, 100)
                                : al_map_rgb(220, 80, 80);

        al_draw_text(font, color, screen_w / 2, screen_h - 120, ALLEGRO_ALIGN_CENTER, label);
        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    selected = 0;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    selected = 1;
                    break;
                case ALLEGRO_KEY_ENTER: {
                    const char *p = (selected == 0) ? PLAYER_MALE_PATH : PLAYER_FEMALE_PATH;
                    snprintf(out_path, (size_t)out_cap, "%s", p);
                    ok = true;
                    running = false;
                    al_rest(0.15); // evita pular tela seguinte
                    break;
                }
                case ALLEGRO_KEY_ESCAPE:
                    ok = false;
                    running = false;
                    break;
            }
        }
    }

    if (font)   al_destroy_font(font);
    if (male)   al_destroy_bitmap(male);
    if (female) al_destroy_bitmap(female);
    al_destroy_event_queue(queue);
    return ok;
}
