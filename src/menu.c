#include "menu.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

int show_menu_screen(ALLEGRO_DISPLAY *display) {
    ALLEGRO_FONT *font = al_load_font("assets/fonts/MedievalSharp.ttf", 36, 0);
    if (!font)
        font = al_create_builtin_font();

    int W = al_get_display_width(display);
    int H = al_get_display_height(display);

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue) { 
        al_destroy_font(font); 
        return -1; 
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    const char *opcoes[] = {"Novo Jogo", "Sair"};
    int opcao_selecionada = 0;
    bool running = true;
    int result = -1;

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            result = -1;
            break;
        }

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP)
                opcao_selecionada = (opcao_selecionada - 1 + 2) % 2;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
                opcao_selecionada = (opcao_selecionada + 1) % 2;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                result = opcao_selecionada;
                break;
            }
        }

        al_clear_to_color(al_map_rgb(10, 10, 20));

        al_draw_text(font, al_map_rgb(255, 255, 150), 
                     W / 2, H * 0.25, ALLEGRO_ALIGN_CENTER, 
                     "Menu Principal");

        for (int i = 0; i < 2; i++) {
            ALLEGRO_COLOR cor = (i == opcao_selecionada)
                                ? al_map_rgb(255, 230, 120)
                                : al_map_rgb(180, 180, 180);
            al_draw_text(font, cor, W / 2, H * (0.45 + i * 0.15), 
                         ALLEGRO_ALIGN_CENTER, opcoes[i]);
        }

        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_event_queue(queue);
    return result;
}
