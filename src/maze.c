#include "maze.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

// Variáveis globais usadas no desenho
float MAZE_TILE_W = TILE_SIZE;
float MAZE_TILE_H = TILE_SIZE;
float MAZE_OFF_X = 0.0f;
float MAZE_OFF_Y = 0.0f;

void maze_load(Maze *maze, int level) {
    // Carrega imagens
    maze->wall_img = al_load_bitmap("assets/sprites/wall.png");
    maze->floor_img = al_load_bitmap("assets/sprites/floor.png");

    maze->portal_img[0] = al_load_bitmap("assets/sprites/portal_1.png");
    maze->portal_img[1] = al_load_bitmap("assets/sprites/portal_2.png");
    maze->portal_img[2] = al_load_bitmap("assets/sprites/portal_3.png");
    maze->portal_img[3] = al_load_bitmap("assets/sprites/portal_4.png");

    if (!maze->wall_img || !maze->floor_img) {
        printf("Erro ao carregar imagens base do labirinto!\n");
    }

    for (int i = 0; i < 4; i++) {
        if (!maze->portal_img[i]) {
            printf("Erro ao carregar portal_%d.png\n", i + 1);
        }
    }

    // Define qual labirinto será usado
    switch (level) {
        case 1: maze1_load(maze->data); break;
        case 2: maze2_load(maze->data); break;
        case 3: maze3_load(maze->data); break;
        case 4: maze4_load(maze->data); break;
        default: maze1_load(maze->data); break;
    }
}

void maze_draw(Maze *maze) {
    float screen_w = al_get_display_width(al_get_current_display());
    float screen_h = al_get_display_height(al_get_current_display());

    float maze_px_w = MAZE_COLS * TILE_SIZE;
    float maze_px_h = MAZE_ROWS * TILE_SIZE;

    // Escala uniforme para caber na tela
    float scale = fminf(screen_w / maze_px_w, screen_h / maze_px_h);

    MAZE_TILE_W = TILE_SIZE * scale;
    MAZE_TILE_H = TILE_SIZE * scale;
    MAZE_OFF_X = (screen_w - (MAZE_TILE_W * MAZE_COLS)) / 2.0f;
    MAZE_OFF_Y = (screen_h - (MAZE_TILE_H * MAZE_ROWS)) / 2.0f;

    al_clear_to_color(al_map_rgb(8, 8, 12));

    for (int y = 0; y < MAZE_ROWS; y++) {
        for (int x = 0; x < MAZE_COLS; x++) {
            int tile = maze->data[y][x];
            float draw_x = MAZE_OFF_X + x * MAZE_TILE_W;
            float draw_y = MAZE_OFF_Y + y * MAZE_TILE_H;

            switch (tile) {
                case T_WALL:
                    if (maze->wall_img)
                        al_draw_scaled_bitmap(maze->wall_img, 0, 0,
                                              al_get_bitmap_width(maze->wall_img),
                                              al_get_bitmap_height(maze->wall_img),
                                              draw_x, draw_y, MAZE_TILE_W, MAZE_TILE_H, 0);
                    break;

                case T_EMPTY:
                    if (maze->floor_img)
                        al_draw_scaled_bitmap(maze->floor_img, 0, 0,
                                              al_get_bitmap_width(maze->floor_img),
                                              al_get_bitmap_height(maze->floor_img),
                                              draw_x, draw_y, MAZE_TILE_W, MAZE_TILE_H, 0);
                    break;

                case T_PORTAL1:
                case T_PORTAL2:
                case T_PORTAL3:
                case T_PORTAL4: {
                    int index = tile - T_PORTAL1;
                    if (maze->floor_img)
                        al_draw_scaled_bitmap(maze->floor_img, 0, 0,
                                              al_get_bitmap_width(maze->floor_img),
                                              al_get_bitmap_height(maze->floor_img),
                                              draw_x, draw_y, MAZE_TILE_W, MAZE_TILE_H, 0);
                    if (maze->portal_img[index])
                        al_draw_scaled_bitmap(maze->portal_img[index], 0, 0,
                                              al_get_bitmap_width(maze->portal_img[index]),
                                              al_get_bitmap_height(maze->portal_img[index]),
                                              draw_x + MAZE_TILE_W * 0.05f,
                                              draw_y + MAZE_TILE_H * 0.05f,
                                              MAZE_TILE_W * 0.9f, MAZE_TILE_H * 0.9f, 0);
                    break;
                }

                default:
                    if (maze->floor_img)
                        al_draw_scaled_bitmap(maze->floor_img, 0, 0,
                                              al_get_bitmap_width(maze->floor_img),
                                              al_get_bitmap_height(maze->floor_img),
                                              draw_x, draw_y, MAZE_TILE_W, MAZE_TILE_H, 0);
                    break;
            }
        }
    }
}

void maze_unload(Maze *maze) {
    if (maze->wall_img) al_destroy_bitmap(maze->wall_img);
    if (maze->floor_img) al_destroy_bitmap(maze->floor_img);

    for (int i = 0; i < 4; i++) {
        if (maze->portal_img[i]) al_destroy_bitmap(maze->portal_img[i]);
    }
}

bool maze_is_wall(Maze *maze, int grid_x, int grid_y) {
    if (grid_x < 0 || grid_x >= MAZE_COLS || grid_y < 0 || grid_y >= MAZE_ROWS)
        return true;
    return maze->data[grid_y][grid_x] == T_WALL;
}
