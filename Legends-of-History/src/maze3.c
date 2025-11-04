#include "maze.h"
#include "config.h"

void maze3_load(int data[MAZE_ROWS][MAZE_COLS]) {
    for (int y = 0; y < MAZE_ROWS; y++)
        for (int x = 0; x < MAZE_COLS; x++)
            data[y][x] = T_EMPTY;

    for (int x = 0; x < MAZE_COLS; x++)
        data[0][x] = data[MAZE_ROWS - 1][x] = T_WALL;
    for (int y = 0; y < MAZE_ROWS; y++)
        data[y][0] = data[y][MAZE_COLS - 1] = T_WALL;

    for (int y = 2; y < MAZE_ROWS - 2; y++)
        for (int x = 2; x < MAZE_COLS - 2; x++)
            if ((x + y) % 5 == 0 || (x - y) % 7 == 0)
                data[y][x] = T_WALL;

    data[MAZE_ROWS / 2][MAZE_COLS - 3] = T_PORTAL3;
}
