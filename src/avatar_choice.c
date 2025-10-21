#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"
#include "game.h"

void avatar_choice_loop() {
    ALLEGRO_DISPLAY *display = al_get_current_display();
    if (!display) {
        fprintf(stderr, "Nenhum display ativo! Inicialize o Allegro no main antes.\n");
        return;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    ALLEGRO_BITMAP *sprite_f = al_load_bitmap(PLAYER_FEMALE_PATH);
    ALLEGRO_BITMAP *sprite_m = al_load_bitmap(PLAYER_MALE_PATH);
    ALLEGRO_FONT *font = al_create_builtin_font();

    if (!sprite_f || !sprite_m) {
        fprintf(stderr, "⚠️ Falha ao carregar sprites de avatar!\n");
        return;
    }

    bool running = true;
    int avatar = 0; // 0 = feminino, 1 = masculino

    while (running) {
        al_clear_to_color(al_map_rgb(15, 15, 30));

        int sprite_width = 96;
        int sprite_height = 96;
        int fx = SCREEN_WIDTH / 4 - sprite_width / 2;
        int mx = 3 * SCREEN_WIDTH / 4 - sprite_width / 2;
        int y = SCREEN_HEIGHT / 2 - sprite_height / 2;

        al_draw_scaled_bitmap(sprite_f, 0, 0,
                              al_get_bitmap_width(sprite_f), al_get_bitmap_height(sprite_f),
                              fx, y, sprite_width, sprite_height, 0);

        al_draw_scaled_bitmap(sprite_m, 0, 0,
                              al_get_bitmap_width(sprite_m), al_get_bitmap_height(sprite_m),
                              mx, y, sprite_width, sprite_height, 0);

        if (avatar == 0)
            al_draw_rectangle(fx - 10, y - 10, fx + sprite_width + 10, y + sprite_height + 10, al_map_rgb(255, 215, 0), 4);
        else
            al_draw_rectangle(mx - 10, y - 10, mx + sprite_width + 10, y + sprite_height + 10, al_map_rgb(255, 215, 0), 4);

        al_draw_text(font, al_map_rgb(255, 255, 255),
                     SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.8,
                     ALLEGRO_ALIGN_CENTER, "← → para escolher | ENTER para confirmar");

        al_flip_display();

        ALLEGRO_EVENT ev;
        if (al_wait_for_event_timed(queue, &ev, 0.05)) {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) avatar = 0;
                if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) avatar = 1;
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    running = false;
                    al_destroy_bitmap(sprite_f);
                    al_destroy_bitmap(sprite_m);
                    al_destroy_font(font);
                    al_destroy_event_queue(queue);
                    start_game(avatar);
                    return;
                }
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    running = false;
            }
        }
    }

    al_destroy_bitmap(sprite_f);
    al_destroy_bitmap(sprite_m);
    al_destroy_font(font);
    al_destroy_event_queue(queue);
}



