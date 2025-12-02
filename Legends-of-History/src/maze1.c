#include "maze.h"
#include "config.h"

void maze1_load(int data[MAZE_ROWS][MAZE_COLS]) {
    for (int y = 0; y < MAZE_ROWS; y++) {
        for (int x = 0; x < MAZE_COLS; x++) {
            data[y][x] = T_EMPTY;
        }
    }

    for (int x = 0; x < MAZE_COLS; x++) {
        data[0][x] = T_WALL;
        data[MAZE_ROWS - 1][x] = T_WALL;
    }
    for (int y = 0; y < MAZE_ROWS; y++) {
        data[y][0] = T_WALL;
        data[y][MAZE_COLS - 1] = T_WALL;
    }

    for (int y = 2; y < MAZE_ROWS - 2; y += 3) {
        for (int x = 1; x < MAZE_COLS - 1; x++) {
            if (x % 10 != 0) // Cria passagens
                data[y][x] = T_WALL;
        }
    }

    for (int x = 5; x < MAZE_COLS - 5; x += 7) {
        for (int y = 3; y < MAZE_ROWS - 3; y++) {
            if (y % 8 != 0)
                data[y][x] = T_WALL;
        }
    }

    for (int x = 1; x < MAZE_COLS - 2; x++) {
        data[MAZE_ROWS - 4][x] = T_EMPTY;
    }

    data[MAZE_ROWS - 4][MAZE_COLS - 3] = T_PORTAL1;

    data[1][1] = T_EMPTY;
}

