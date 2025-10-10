#ifndef ENEMIES_H
#define ENEMIES_H

#define MAX_ENEMIES 3

typedef struct {
    int x, y;
    int dir; 
} Enemy;

void enemies_init();
void enemies_update();
void enemies_draw();
int check_player_collision(int px, int py);

#endif