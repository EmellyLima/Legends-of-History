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

// Inicializa todos os módulos Allegro necessários
static bool init_allegro_all(void) {
    if (!al_init()) return false;
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(8);
    return true;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    if (!init_allegro_all()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return 1;
    }

    int W = 1280, H = 720;
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    ALLEGRO_DISPLAY *display = al_create_display(W, H);
    if (!display) {
        fprintf(stderr, "Falha ao criar display.\n");
        return 1;
    }

    al_set_window_title(display, "Legends of History");
    printf("Iniciando Allegro...\n");
    printf("Display criado com sucesso.\n");

    bool running = true;

    while (running) {
        // Tela de logo e menu
        printf("Mostrando logo...\n");
        show_logo_screen(display);
        printf("Mostrando menu inicial...\n");
        show_intro_screen(display);

        // Escolha de avatar
        printf("Escolhendo avatar...\n");
        char avatar_path[256] = {0};
        if (!avatar_choice_loop(display, avatar_path, sizeof(avatar_path))) {
            printf("Escolha de avatar cancelada. Voltando ao menu.\n");
            continue; // volta ao menu sem fechar o jogo
        }

        // Nome do jogador
        printf("Inserindo nome do jogador...\n");
        char player_name[64] = {0};
        if (!player_name_loop(display, player_name, sizeof(player_name))) {
            printf("Jogador cancelou nome. Voltando ao menu.\n");
            continue; // volta ao menu em vez de encerrar
        }

        // Inicia o jogo
        printf("Iniciando jogo...\n");
        game_loop(avatar_path, player_name);

        // Após o jogo, retorna ao menu novamente
        printf("Jogo encerrado. Voltando ao menu principal...\n");
    }

    al_destroy_display(display);
    al_uninstall_audio();
    al_uninstall_system();

    printf("[DEBUG] Finalização concluída.\n");
    return 0;
}
