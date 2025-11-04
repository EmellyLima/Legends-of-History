#include "maze.h"
#include "config.h"

void maze4_load(int data[MAZE_ROWS][MAZE_COLS]) {
    for (int y = 0; y < MAZE_ROWS; y++)
        for (int x = 0; x < MAZE_COLS; x++)
            data[y][x] = T_EMPTY;

    for (int x = 0; x < MAZE_COLS; x++)
        data[0][x] = data[MAZE_ROWS - 1][x] = T_WALL;
    for (int y = 0; y < MAZE_ROWS; y++)
        data[y][0] = data[y][MAZE_COLS - 1] = T_WALL;

    for (int y = 2; y < MAZE_ROWS - 2; y++)
        for (int x = 2; x < MAZE_COLS - 2; x++)
            if ((x * y) % 9 == 0 || (x + y) % 8 == 0)
                data[y][x] = T_WALL;

    for (int y = 4; y < MAZE_ROWS - 4; y += 5)
        for (int x = 3; x < MAZE_COLS - 3; x += 4)
            if ((x + y) % 2 == 0)
                data[y][x] = T_WALL;

    data[MAZE_ROWS - 4][MAZE_COLS - 4] = T_PORTAL4;
}
