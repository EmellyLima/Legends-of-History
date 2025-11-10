#include "maze3_map.h"
#include "config.h"

void maze3_load(int data[MAZE_ROWS][MAZE_COLS]) {
    for (int y = 0; y < MAZE_ROWS; y++) {
        for (int x = 0; x < MAZE_COLS; x++) {

            // Protege contra acessos fora dos limites do mapa
            int value = (x < MAZE_COLS && y < MAZE_ROWS) ? maze3_layout[y][x] : 0;

            switch (value) {
                case 1:
                    data[y][x] = T_WALL;
                    break;
                case 2:
                    data[y][x] = T_PORTAL3;
                    break;
                case 3:
                    data[y][x] = T_PORTAL4; // compatível para mapas futuros
                    break;
                default:
                    data[y][x] = T_FLOOR;
                    break;
            }
        }
    }
}
