#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int x, y;
} Player;

void player_init(Player *p, int start_x, int start_y);
void player_update(Player *p, int next_x, int next_y);
void player_draw(Player *p);

#endif