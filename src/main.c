#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "logo.h"
#include "intro.h"
#include "avatar_choice.h"
#include "player_name.h"
#include "game.h"

/* Inicializa tudo do Allegro */
static bool init_allegro_all(void) {
    if (!al_init()) return false;

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();

    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(16);

    return true;
}

int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    if (!init_allegro_all()) {
        fprintf(stderr, "Falha ao iniciar Allegro.\n");
        return 1;
    }

    int W = 1280, H = 720;
    al_set_new_display_flags(ALLEGRO_WINDOWED);

    ALLEGRO_DISPLAY *display = al_create_display(W, H);
    if (!display) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return 1;
    }

    al_set_window_title(display, "Legends of History");

    bool running = true;

    while (running)
    {
        /* Tela de logo */
        show_logo_screen(display);

        /* Menu — retorna true se jogador escolheu “Jogar” */
        bool continuar = show_intro_screen(display);

        /* Se apertou ESC no menu → encerra o jogo */
        if (!continuar) {
            running = false;
            break;
        }

        /* Escolha de avatar */
        char avatar_path[256] = {0};
        if (!avatar_choice_loop(display, avatar_path, sizeof(avatar_path))) {
            /* ESC → volta ao menu */
            continue;
        }

        /* Nome do jogador */
        char player_name[64] = {0};
        if (!player_name_loop(display, player_name, sizeof(player_name))) {
            /* ESC → volta ao menu */
            continue;
        }

        /*
            Inicia o jogo.
            IMPORTANTE: game_loop NÃO fecha o programa.
            Se o jogador morrer ou apertar ESC no jogo, apenas retorna aqui.
        */
        game_loop(avatar_path, player_name);

        /*
            Quando o game_loop termina, volta para o MENU,
            sem fechar o programa.
        */
    }

    al_destroy_display(display);
    al_uninstall_audio();
    al_uninstall_system();
    return 0;
}
