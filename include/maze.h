#ifndef MAZE_H
#define MAZE_H

#include "config.h"

extern int mapa[LINHAS][COLUNAS];

void maze_init();
void maze_draw();
int maze_is_free(int x, int y);

#endif
