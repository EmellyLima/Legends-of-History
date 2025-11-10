#include "player_name.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

bool player_name_loop(ALLEGRO_DISPLAY *display, char *out_name, int out_cap) {
    if (!display || !out_name || out_cap <= 1) return false;

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue) return false;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    // fontes
    ALLEGRO_FONT *font_title = al_load_font("assets/fonts/font.ttf", 28, 0);
    ALLEGRO_FONT *font_input = al_load_font("assets/fonts/font.ttf", 24, 0);
    if (!font_title) font_title = al_create_builtin_font();
    if (!font_input) font_input = al_create_builtin_font();

    // medidas da tela
    const int W = al_get_display_width(display);
    const int H = al_get_display_height(display);

    // métrica da caixa de input (centralizada)
    const float box_w = (W * 0.60f > 700.0f) ? 700.0f : (W * 0.60f);
    const float box_h = 72.0f;
    const float cx = W * 0.5f;
    const float cy = H * 0.50f;      // altura da caixa
    const float box_l = cx - box_w * 0.5f;
    const float box_t = cy - box_h * 0.5f;
    const float box_r = cx + box_w * 0.5f;
    const float box_b = cy + box_h * 0.5f;

    char buffer[128] = "";
    int len = 0;
    bool running = true;
    bool confirmed = false;

    double last_blink = al_get_time();
    bool show_cursor = true;

    while (running) {
        // pisca cursor
        double now = al_get_time();
        if (now - last_blink >= 0.5) {
            show_cursor = !show_cursor;
            last_blink = now;
        }

        // fundo
        al_clear_to_color(al_map_rgb(12, 12, 16));

        // título (acima da caixa)
        al_draw_text(font_title, al_map_rgb(220, 60, 60),
                     cx, cy - box_h - 40, ALLEGRO_ALIGN_CENTER,
                     "DIGITE SEU NOME:");

        // caixa
        al_draw_filled_rounded_rectangle(box_l, box_t, box_r, box_b, 18, 18, al_map_rgb(22, 24, 32));
        al_draw_rounded_rectangle(box_l, box_t, box_r, box_b, 18, 18, al_map_rgb(210, 80, 80), 3);

        // texto do input centralizado na caixa
        char display_text[140];
        snprintf(display_text, sizeof(display_text), "%s%s", buffer, show_cursor ? "_" : "");
        al_draw_text(font_input, al_map_rgb(240, 240, 240),
                     cx, cy - al_get_font_line_height(font_input) * 0.5f,
                     ALLEGRO_ALIGN_CENTER, display_text);

        // instruções
        al_draw_text(font_input, al_map_rgb(160,160,160),
                     cx, H * 0.75f, ALLEGRO_ALIGN_CENTER,
                     "ENTER: confirmar   |   ESC: voltar");

        al_flip_display();

        // eventos
        ALLEGRO_EVENT ev;
        if (al_wait_for_event_timed(queue, &ev, 0.05)) {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
                confirmed = false;
            } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                int key = ev.keyboard.keycode;

                if (key == ALLEGRO_KEY_ESCAPE) {
                    running = false;
                    confirmed = false;   // volta ao menu
                } else if (key == ALLEGRO_KEY_ENTER) {
                    if (len > 0) {
                        running = false;
                        confirmed = true;
                    }
                } else if (key == ALLEGRO_KEY_BACKSPACE) {
                    if (len > 0) {
                        buffer[--len] = '\0';
                    }
                }
            } else if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
                int unicode = ev.keyboard.unichar;
                if (isprint(unicode) && len < (int)sizeof(buffer) - 1) {
                    // evita quebras de linha e TAB
                    if (unicode != '\n' && unicode != '\r' && unicode != '\t') {
                        buffer[len++] = (char)unicode;
                        buffer[len] = '\0';
                    }
                }
            }
        }
    }

    if (confirmed) {
        strncpy(out_name, buffer, out_cap);
        out_name[out_cap - 1] = '\0';
    }

    if (font_title && font_title != al_create_builtin_font()) al_destroy_font(font_title);
    if (font_input && font_input != al_create_builtin_font()) al_destroy_font(font_input);
    al_destroy_event_queue(queue);
    return confirmed;
}
