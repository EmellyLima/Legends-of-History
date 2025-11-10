#include "maze1_map.h"
#include "config.h"

void maze1_load(int data[MAZE_ROWS][MAZE_COLS]) {
    for (int y = 0; y < MAZE_ROWS; y++) {
        for (int x = 0; x < MAZE_COLS; x++) {

            // Evita acesso fora do limite do layout
            int value = (x < MAZE_COLS && y < MAZE_ROWS) ? maze1_layout[y][x] : 0;

            switch (value) {
                case 1: data[y][x] = T_WALL;    break;
                case 0: data[y][x] = T_FLOOR;   break;
                case 2: data[y][x] = T_PORTAL1; break;
                case 3: data[y][x] = T_PORTAL2; break;
                case 4: data[y][x] = T_PORTAL3; break;
                case 5: data[y][x] = T_PORTAL4; break;
                default:
                    data[y][x] = T_FLOOR;
                    break;
            }
        }
    }
}
