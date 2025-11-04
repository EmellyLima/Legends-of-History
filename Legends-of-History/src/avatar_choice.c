#include "avatar_choice.h"
#include "game.h"
#include "config.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

void avatar_choice_loop() {
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

    ALLEGRO_FONT *font = al_load_font("assets/fonts/font.ttf", 32, 0);

    ALLEGRO_BITMAP *male = al_load_bitmap(PLAYER_MALE_PATH);
    ALLEGRO_BITMAP *female = al_load_bitmap(PLAYER_FEMALE_PATH);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    int selected = 0;
    bool running = true;

    while (running) {
        al_clear_to_color(al_map_rgb(15, 15, 25));
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 80, ALLEGRO_ALIGN_CENTER, "Escolha seu personagem:");

        al_draw_bitmap(male, SCREEN_WIDTH / 2 - 300, 250, 0);
        al_draw_bitmap(female, SCREEN_WIDTH / 2 + 100, 250, 0);

        if (selected == 0)
            al_draw_text(font, al_map_rgb(255, 255, 0), SCREEN_WIDTH / 2 - 220, 600, 0, "Selecionado: MASCULINO");
        else
            al_draw_text(font, al_map_rgb(255, 255, 0), SCREEN_WIDTH / 2 + 180, 600, ALLEGRO_ALIGN_CENTER, "Selecionado: FEMININO");

        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
                selected = 0;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                selected = 1;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                running = false;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                return;
        }
    }

    const char *avatar = selected == 0 ? PLAYER_MALE_PATH : PLAYER_FEMALE_PATH;
    game_loop(avatar);

    al_destroy_font(font);
    al_destroy_bitmap(male);
    al_destroy_bitmap(female);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
}
