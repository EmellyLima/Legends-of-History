#ifndef PLAYER_NAME_H
#define PLAYER_NAME_H

#include <allegro5/allegro.h>
#include <stdbool.h>

// Exibe a tela para o jogador digitar o nome.
// Retorna false se cancelar (ESC), true se confirmar (ENTER).
bool player_name_loop(ALLEGRO_DISPLAY *display, char *out_name, int out_cap);

#endif
