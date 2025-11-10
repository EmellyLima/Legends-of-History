#include "maze4_map.h"
#include "config.h"

void maze4_load(int data[MAZE_ROWS][MAZE_COLS]) {
    for (int y = 0; y < MAZE_ROWS; y++) {
        for (int x = 0; x < MAZE_COLS; x++) {
            int value = (x < MAZE_COLS && y < MAZE_ROWS) ? maze4_layout[y][x] : 0;

            switch (value) {
                case 1:
                    data[y][x] = T_WALL;
                    break;
                case 2:
                    data[y][x] = T_PORTAL4;
                    break;
                default:
                    data[y][x] = T_FLOOR;
                    break;
            }
        }
    }

    // Garante que o portal final esteja no topo central (ajuste seguro)
    if (MAZE_COLS > 2 && MAZE_ROWS > 0)
        data[0][MAZE_COLS / 2] = T_PORTAL4;
}
