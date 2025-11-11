#include "intro.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdbool.h>

// Função auxiliar: desenha um botão com destaque quando selecionado
static void draw_button(float x, float y, float w, float h, bool selected, const char *label, ALLEGRO_FONT *font)
{
    ALLEGRO_COLOR border = selected ? al_map_rgb(255, 200, 50) : al_map_rgb(120, 120, 120);
    ALLEGRO_COLOR fill = al_map_rgba(25, 25, 30, 230);
    al_draw_filled_rounded_rectangle(x, y, x + w, y + h, 12, 12, fill);
    al_draw_rounded_rectangle(x, y, x + w, y + h, 12, 12, border, 2);
    al_draw_text(font, selected ? al_map_rgb(255, 220, 100) : al_map_rgb(230, 230, 230),
                 x + w / 2, y + h / 2 - al_get_font_line_height(font) / 2,
                 ALLEGRO_ALIGN_CENTER, label);
}

// Tela de regras (volta pro menu ao apertar ESC)
static void show_rules_screen(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, ALLEGRO_FONT *title_font)
{
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    bool show = true;
    while (show)
    {
        al_clear_to_color(al_map_rgb(10, 10, 10));
        int W = al_get_display_width(display);
        int H = al_get_display_height(display);

        al_draw_text(title_font, al_map_rgb(255, 220, 100), W / 2, 80, ALLEGRO_ALIGN_CENTER, "REGRAS DO JOGO");

        float y = 180;
        al_draw_text(font, al_map_rgb(230, 230, 230), W / 2, y, ALLEGRO_ALIGN_CENTER,
                     "1. Use as SETAS para mover e ESPAÇO para atacar.");
        al_draw_text(font, al_map_rgb(230, 230, 230), W / 2, y + 40, ALLEGRO_ALIGN_CENTER,
                     "2. Enfrente inimigos e resolva enigmas históricos.");
        al_draw_text(font, al_map_rgb(230, 230, 230), W / 2, y + 80, ALLEGRO_ALIGN_CENTER,
                     "3. Alcance o portal e responda perguntas para avançar.");
        al_draw_text(font, al_map_rgb(230, 230, 230), W / 2, y + 120, ALLEGRO_ALIGN_CENTER,
                     "4. Ganhe vidas extras ao derrotar inimigos (máx. 5).");

        al_draw_text(font, al_map_rgb(255, 255, 180), W / 2, H - 100, ALLEGRO_ALIGN_CENTER,
                     "Pressione ESC para voltar ao menu.");

        al_flip_display();

        ALLEGRO_EVENT ev;
        if (al_wait_for_event_timed(queue, &ev, 0.05))
        {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                show = false; // volta pro menu
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                exit(0);
        }
    }

    al_destroy_event_queue(queue);
}

// Tela principal (menu)
void show_intro_screen(ALLEGRO_DISPLAY *display)
{
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(4);

    // Carrega recursos visuais
    ALLEGRO_BITMAP *logo = al_load_bitmap("assets/sprites/logo.png");
    if (!logo)
        printf("[AVISO] Logo não encontrada.\n");

    // Carrega música
    ALLEGRO_SAMPLE *music = al_load_sample("assets/audio/intro.ogg");
    if (music)
        al_play_sample(music, 0.8, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    else
        printf("[AVISO] Música não encontrada (intro.ogg).\n");

    // Fontes
    ALLEGRO_FONT *title_font = al_load_font("assets/fonts/font.ttf", 48, 0);
    ALLEGRO_FONT *menu_font = al_load_font("assets/fonts/font.ttf", 28, 0);
    if (!title_font || !menu_font)
    {
        printf("[AVISO] Falha ao carregar fontes. Usando padrão.\n");
        title_font = al_create_builtin_font();
        menu_font = al_create_builtin_font();
    }

    // Opções do menu
    const char *options[] = {"Jogar", "Regras", "Som: Ligado", "Sair"};
    const int num_options = 4;
    int selected = 0;
    bool running = true;
    bool music_on = true;

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    int W = al_get_display_width(display);
    int H = al_get_display_height(display);

    while (running)
    {
        al_clear_to_color(al_map_rgb(10, 10, 15));

        // Logo no canto superior esquerdo
        if (logo)
        {
            int lw = al_get_bitmap_width(logo);
            int lh = al_get_bitmap_height(logo);
            float scale = 0.25f;
            al_draw_scaled_bitmap(logo, 0, 0, lw, lh, 60, 60, lw * scale, lh * scale, 0);
        }

        // Título
        al_draw_text(title_font, al_map_rgb(255, 220, 100), W / 2, H * 0.25f, ALLEGRO_ALIGN_CENTER, "Legends of History");

        // Botões
        float btn_w = 280, btn_h = 50;
        float start_y = H * 0.45f;
        for (int i = 0; i < num_options; i++)
        {
            draw_button(W / 2 - btn_w / 2, start_y + i * 70, btn_w, btn_h,
                        i == selected, options[i], menu_font);
        }

        al_flip_display();

        ALLEGRO_EVENT ev;
        if (al_wait_for_event_timed(queue, &ev, 0.05))
        {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch (ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_DOWN:
                    selected = (selected + 1) % num_options;
                    break;
                case ALLEGRO_KEY_UP:
                    selected = (selected - 1 + num_options) % num_options;
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (selected == 0)
                    {
                        running = false; // Jogar
                    }
                    else if (selected == 1)
                    {
                        show_rules_screen(display, menu_font, title_font);
                        selected = 0; // volta com foco no topo (Jogar)

                        // limpa o estado do teclado e eventos antigos
                        ALLEGRO_EVENT clear_ev;
                        while (al_get_next_event(queue, &clear_ev))
                        {
                        }
                        al_rest(0.1); // pequena pausa pra garantir que o ESC não ative o "Jogar"
                    }

                    else if (selected == 2)
                    {
                        music_on = !music_on;
                        options[2] = music_on ? "Som: Ligado" : "Som: Desligado";
                        if (music_on && music)
                            al_play_sample(music, 0.8, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                        else
                            al_stop_samples();
                    }
                    else if (selected == 3)
                    {
                        exit(0);
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
                }
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                running = false;
            }
        }
    }

    // Libera recursos
    if (music)
        al_destroy_sample(music);
    if (logo)
        al_destroy_bitmap(logo);
    al_destroy_font(title_font);
    al_destroy_font(menu_font);
    al_destroy_event_queue(queue);
}
