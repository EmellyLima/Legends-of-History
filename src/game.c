#include "game.h"
#include "maze.h"
#include "portal.h"
#include "player.h"
#include "enemies.h"
#include "config.h"
#include <stdio.h>
#include <stdbool.h> 
#include <allegro5/allegro_primitives.h>

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila = NULL;
ALLEGRO_TIMER *timer = NULL;

void iniciar_jogo() {
    if (!al_init()) {
        printf("Erro ao iniciar o Allegro.\n");
        return;
    }
    al_init_primitives_addon();
    al_install_keyboard();

    janela = al_create_display(COLUNAS * TAM, LINHAS * TAM);
    if (!janela) {
        printf("Erro ao criar a janela.\n");
        return;
    }
    fila = al_create_event_queue();
    timer = al_create_timer(1.0 / 30.0); 

    maze_init();
    portals_init();
    enemies_init();
    
    Player jogador;
    player_init(&jogador, 1, 1);

    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());

    bool sair = false;
    bool redesenhar = true;
    int tecla_pressionada = 0; // Para evitar movimento contínuo

    al_start_timer(timer);

    while (!sair) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true;
        } 
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            tecla_pressionada = ev.keyboard.keycode; // Armazena a tecla pressionada
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            tecla_pressionada = 0; // Limpa quando a tecla é solta
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            int proximo_x = jogador.x;
            int proximo_y = jogador.y;

            if (tecla_pressionada == ALLEGRO_KEY_ESCAPE) sair = true;
            if (tecla_pressionada == ALLEGRO_KEY_LEFT)   proximo_x--;
            if (tecla_pressionada == ALLEGRO_KEY_RIGHT)  proximo_x++;
            if (tecla_pressionada == ALLEGRO_KEY_UP)     proximo_y--;
            if (tecla_pressionada == ALLEGRO_KEY_DOWN)   proximo_y++;

            // Se uma tecla de movimento foi pressionada, atualiza e limpa
            if (proximo_x != jogador.x || proximo_y != jogador.y) {
                player_update(&jogador, proximo_x, proximo_y);
                tecla_pressionada = 0; // Evita que o jogador deslize
            }

            enemies_update();

            // --- VERIFICAÇÃO DE COLISÕES E INTERAÇÕES ---
            if (check_player_collision(jogador.x, jogador.y)) {
                printf("Colidiu com um inimigo! Fim de jogo.\n");
                al_rest(2.0); // Pausa por 2 segundos
                sair = true;
            }

            int portal_id = check_player_on_portal(jogador.x, jogador.y);
            if (portal_id != -1) {
                printf("Jogador entrou no portal %d!\n", portal_id + 1);
                // Futuramente, adicionar lógica de teletransporte aqui
            }
            
            redesenhar = true;
        }

        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            maze_draw();
            portals_draw();
            enemies_draw();
            player_draw(&jogador);

            al_flip_display();
        }
    }

    al_destroy_display(janela);
    al_destroy_event_queue(fila);
    al_destroy_timer(timer);
}