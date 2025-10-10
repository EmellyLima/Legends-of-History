#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>

// Variáveis globais do Allegro
extern ALLEGRO_DISPLAY *janela;
extern ALLEGRO_EVENT_QUEUE *fila;
extern ALLEGRO_TIMER *timer;

void iniciar_jogo();

#endif
