// include/avatar_choice.h
#ifndef AVATAR_CHOICE_H
#define AVATAR_CHOICE_H

#include <allegro5/allegro.h>
#include <stdbool.h>

// retorna false se o jogador cancelar (ESC/fechar)
bool avatar_choice_loop(ALLEGRO_DISPLAY *display, char *out_path, int out_cap);

#endif
