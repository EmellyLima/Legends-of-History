#include "intro.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>

static void draw_button(float x, float y, float w, float h, bool selected, const char *label, ALLEGRO_FONT *font)
{
    ALLEGRO_COLOR border = selected ? al_map_rgb(255, 200, 50) : al_map_rgb(140, 140, 140);
    ALLEGRO_COLOR fill   = al_map_rgba(25, 25, 30, 220);

    al_draw_filled_rounded_rectangle(x, y, x + w, y + h, 10, 10, fill);
    al_draw_rounded_rectangle(x, y, x + w, y + h, 10, 10, border, 2);

    al_draw_text(font, selected ? al_map_rgb(255, 220, 80) : al_map_rgb(230, 230, 230),
                 x + w / 2, y + (h - al_get_font_line_height(font)) / 2,
                 ALLEGRO_ALIGN_CENTER, label);
}

static void show_rules_screen(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_FONT *title_font)
{
    ALLEGRO_EVENT_QUEUE *q = al_create_event_queue();
    al_register_event_source(q, al_get_keyboard_event_source());
    al_register_event_source(q, al_get_display_event_source(display));

    bool running = true;

    while (running)
    {
        al_clear_to_color(al_map_rgb(10, 10, 10));
        int W = al_get_display_width(display);
        int H = al_get_display_height(display);

        al_draw_text(title_font, al_map_rgb(255, 220, 80), W / 2, 80, ALLEGRO_ALIGN_CENTER, "REGRAS DO JOGO");

        float y = 200;
        al_draw_text(font, al_map_rgb(230, 230, 230), W / 2, y,      ALLEGRO_ALIGN_CENTER, "• Use as setas para mover.");
        al_draw_text(font, al_map_rgb(230, 230, 230), W / 2, y + 40, ALLEGRO_ALIGN_CENTER, "• Ataque com ESPAÇO.");
        al_draw_text(font, al_map_rgb(230, 230, 230), W / 2, y + 80, ALLEGRO_ALIGN_CENTER, "• Alcance o portal e responda o enigma.");
        al_draw_text(font, al_map_rgb(230, 230, 230), W / 2, y + 120,ALLEGRO_ALIGN_CENTER, "• Derrote inimigos para ganhar vida extra.");

        al_draw_text(font, al_map_rgb(255, 255, 180),
                     W / 2, H - 120, ALLEGRO_ALIGN_CENTER, "Aperte ESC para voltar ao menu");

        al_flip_display();

        ALLEGRO_EVENT e;
        if (al_wait_for_event_timed(q, &e, 0.05))
        {
            if (e.type == ALLEGRO_EVENT_KEY_DOWN &&
                e.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                running = false;
            }

            if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                running = false;
            }
        }
    }

    al_destroy_event_queue(q);
}

bool show_intro_screen(ALLEGRO_DISPLAY *display)
{
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(8);

    ALLEGRO_BITMAP *logo = al_load_bitmap("assets/sprites/logo.png");
    ALLEGRO_SAMPLE *music = al_load_sample("assets/audio/intro.ogg");

    if (music)
        al_play_sample(music, 0.9, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    ALLEGRO_FONT *title_font = al_load_font("assets/fonts/font.ttf", 50, 0);
    ALLEGRO_FONT *menu_font  = al_load_font("assets/fonts/font.ttf", 28, 0);

    if (!title_font || !menu_font)
    {
        title_font = al_create_builtin_font();
        menu_font  = al_create_builtin_font();
    }

    const char *opts[] = {"Jogar", "Regras", "Som: Ligado", "Sair"};
    int selected = 0;
    int opt_count = 4;
    bool music_on = true;

    ALLEGRO_EVENT_QUEUE *q = al_create_event_queue();
    al_register_event_source(q, al_get_keyboard_event_source());
    al_register_event_source(q, al_get_display_event_source(display));

    int W = al_get_display_width(display);
    int H = al_get_display_height(display);

    bool running = true;

    while (running)
    {
        al_clear_to_color(al_map_rgb(10, 10, 15));

        if (logo)
        {
            int lw = al_get_bitmap_width(logo);
            int lh = al_get_bitmap_height(logo);
            float s = 0.23f;

            al_draw_scaled_bitmap(logo, 0, 0, lw, lh, 50, 40, lw * s, lh * s, 0);
        }

        al_draw_text(title_font, al_map_rgb(255, 220, 90),
                     W / 2, H * 0.25f, ALLEGRO_ALIGN_CENTER,
                     "Legends of History");

        float bw = 280, bh = 55;
        float start = H * 0.46f;

        for (int i = 0; i < opt_count; i++)
        {
            draw_button(W / 2 - bw / 2, start + i * 70, bw, bh,
                        i == selected, opts[i], menu_font);
        }

        al_flip_display();

        ALLEGRO_EVENT e;
        if (al_wait_for_event_timed(q, &e, 0.05))
        {
            if (e.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch (e.keyboard.keycode)
                {
                    case ALLEGRO_KEY_DOWN:
                        selected = (selected + 1) % opt_count;
                        break;

                    case ALLEGRO_KEY_UP:
                        selected = (selected - 1 + opt_count) % opt_count;
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        running = false;
                        return false;

                    case ALLEGRO_KEY_ENTER:

                        if (selected == 0)
                        {
                            /* Limpa eventos antigos */
                            ALLEGRO_EVENT dump;
                            while (al_get_next_event(q, &dump)) {}
                            return true;
                        }

                        if (selected == 1)
                        {
                            show_rules_screen(display, menu_font, title_font);

                            ALLEGRO_EVENT dump;
                            while (al_get_next_event(q, &dump)) {}

                            selected = 0;
                        }

                        if (selected == 2)
                        {
                            music_on = !music_on;
                            opts[2] = music_on ? "Som: Ligado" : "Som: Desligado";

                            if (music_on)
                                al_play_sample(music, 0.9, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                            else
                                al_stop_samples();
                        }

                        if (selected == 3)
                        {
                            running = false;
                            return false;
                        }

                        break;
                }
            }
        }
    }

    if (music) al_destroy_sample(music);
    if (logo)  al_destroy_bitmap(logo);
    al_destroy_event_queue(q);

    return false;
}
