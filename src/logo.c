#include "logo.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

void show_logo_screen(ALLEGRO_DISPLAY *display) {
    ALLEGRO_FONT *press_font = al_load_font("assets/fonts/MedievalSharp.ttf", 30, 0);
    if (!press_font) press_font = al_create_builtin_font();

    ALLEGRO_BITMAP *logo = al_load_bitmap("assets/sprites/logo.jpeg");
    int W = al_get_display_width(display);
    int H = al_get_display_height(display);

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    bool done = false;
    while (!done) {
        // Fundo escurecido
        al_draw_filled_rectangle(0, 0, W, H, al_map_rgba(0, 0, 0, 180));

        // Logo centralizado
        if (logo) {
            int lw = al_get_bitmap_width(logo);
            int lh = al_get_bitmap_height(logo);
            float scale = fmin((float)W / lw, (float)H / lh) * 0.6f;
            al_draw_scaled_bitmap(
                logo, 0, 0, lw, lh,
                W / 2 - (lw * scale / 2),
                H / 2 - (lh * scale / 2),
                lw * scale, lh * scale, 0
            );
        }

        // Texto de instrução
        ALLEGRO_COLOR sombra = al_map_rgba(0, 0, 0, 200);
        ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);

        const char *msg = "Pressione ENTER para continuar";
        al_draw_text(press_font, sombra, W / 2 + 2, H * 0.85f + 2, ALLEGRO_ALIGN_CENTER, msg);
        al_draw_text(press_font, branco, W / 2, H * 0.85f, ALLEGRO_ALIGN_CENTER, msg);

        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                done = true;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                break;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
    }

    al_destroy_event_queue(queue);
    al_destroy_font(press_font);
    if (logo) al_destroy_bitmap(logo);
}
