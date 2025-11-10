#include "maze2_map.h"
#include "config.h"

// Carrega o mapa da fase 2 (labirinto quadrado com portal)
void maze2_load(int data[MAZE_ROWS][MAZE_COLS]) {
    for (int y = 0; y < MAZE_ROWS; y++) {
        for (int x = 0; x < MAZE_COLS; x++) {

            // Evita acessar fora do tamanho do mapa
            int value = (x < MAZE_COLS && y < MAZE_ROWS) ? maze2_layout[y][x] : 0;

            switch (value) {
                case 1:
                    data[y][x] = T_WALL;
                    break;
                case 2:
                    data[y][x] = T_PORTAL2;
                    break;
                case 3:
                    data[y][x] = T_PORTAL3;
                    break;
                case 4:
                    data[y][x] = T_PORTAL4;
                    break;
                default:
                    data[y][x] = T_FLOOR;
                    break;
            }
        }
    }
}
