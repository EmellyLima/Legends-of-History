#include "maze.h"
#include <stdio.h>
#include <math.h>

float MAZE_TILE_W = TILE_SIZE;
float MAZE_TILE_H = TILE_SIZE;
float MAZE_OFF_X = 0.0f;
float MAZE_OFF_Y = 0.0f;

// Carrega mapa da fase e texturas
void maze_load(Maze *maze, int level) {
    maze->wall_img  = al_load_bitmap("assets/sprites/wall.png");
    maze->floor_img = al_load_bitmap("assets/sprites/floor.png");

    maze->portal_img[0] = al_load_bitmap("assets/sprites/portal_1.png");
    maze->portal_img[1] = al_load_bitmap("assets/sprites/portal_2.png");
    maze->portal_img[2] = al_load_bitmap("assets/sprites/portal_3.png");
    maze->portal_img[3] = al_load_bitmap("assets/sprites/portal_4.png");

    if (!maze->wall_img || !maze->floor_img)
        printf("[ERRO] Falha ao carregar sprites de parede/piso.\n");

    for (int i = 0; i < 4; i++) {
        if (!maze->portal_img[i])
            printf("[ERRO] Falha ao carregar portal_%d.png\n", i + 1);
    }

    switch (level) {
        case 1: maze1_load(maze->data); break;
        case 2: maze2_load(maze->data); break;
        case 3: maze3_load(maze->data); break;
        case 4: maze4_load(maze->data); break;
        default: maze1_load(maze->data); break;
    }

    maze->completed = false;

    // Calcula centralização e escala apenas uma vez
    ALLEGRO_DISPLAY *display = al_get_current_display();
    if (display) {
        float screen_w = al_get_display_width(display);
        float screen_h = al_get_display_height(display);

        float maze_px_w = MAZE_COLS * TILE_SIZE;
        float maze_px_h = MAZE_ROWS * TILE_SIZE;

        float scale_x = screen_w / maze_px_w;
        float scale_y = screen_h / maze_px_h;
        float scale   = fminf(scale_x, scale_y) * 0.95f;

        MAZE_TILE_W = TILE_SIZE * scale;
        MAZE_TILE_H = TILE_SIZE * scale;

        MAZE_OFF_X = (screen_w - (MAZE_TILE_W * MAZE_COLS)) / 2.0f;
        MAZE_OFF_Y = (screen_h - (MAZE_TILE_H * MAZE_ROWS)) / 2.0f;
    }
}

// Desenha o labirinto na tela
void maze_draw(Maze *maze) {
    for (int y = 0; y < MAZE_ROWS; y++) {
        for (int x = 0; x < MAZE_COLS; x++) {
            int tile = maze->data[y][x];
            float draw_x = MAZE_OFF_X + (x * MAZE_TILE_W);
            float draw_y = MAZE_OFF_Y + (y * MAZE_TILE_H);

            switch (tile) {
                case T_WALL:
                    if (maze->wall_img)
                        al_draw_scaled_bitmap(
                            maze->wall_img, 0, 0,
                            al_get_bitmap_width(maze->wall_img),
                            al_get_bitmap_height(maze->wall_img),
                            draw_x, draw_y, MAZE_TILE_W, MAZE_TILE_H, 0
                        );
                    break;

                case T_PORTAL1:
                case T_PORTAL2:
                case T_PORTAL3:
                case T_PORTAL4: {
                    int idx = tile - T_PORTAL1;
                    if (maze->floor_img)
                        al_draw_scaled_bitmap(
                            maze->floor_img, 0, 0,
                            al_get_bitmap_width(maze->floor_img),
                            al_get_bitmap_height(maze->floor_img),
                            draw_x, draw_y, MAZE_TILE_W, MAZE_TILE_H, 0
                        );
                    if (maze->portal_img[idx])
                        al_draw_scaled_bitmap(
                            maze->portal_img[idx], 0, 0,
                            al_get_bitmap_width(maze->portal_img[idx]),
                            al_get_bitmap_height(maze->portal_img[idx]),
                            draw_x + (MAZE_TILE_W * 0.15f),
                            draw_y + (MAZE_TILE_H * 0.15f),
                            MAZE_TILE_W * 0.7f,
                            MAZE_TILE_H * 0.7f, 0
                        );
                    break;
                }

                default: // piso
                    if (maze->floor_img)
                        al_draw_scaled_bitmap(
                            maze->floor_img, 0, 0,
                            al_get_bitmap_width(maze->floor_img),
                            al_get_bitmap_height(maze->floor_img),
                            draw_x, draw_y, MAZE_TILE_W, MAZE_TILE_H, 0
                        );
                    break;
            }
        }
    }
}

// Impede o jogador de atravessar paredes
bool maze_is_wall(Maze *maze, int grid_x, int grid_y) {
    if (grid_x < 0 || grid_x >= MAZE_COLS || grid_y < 0 || grid_y >= MAZE_ROWS)
        return true;
    return maze->data[grid_y][grid_x] == T_WALL;
}

/* PORTAL / TILE UTILS */
int maze_tile_at(Maze *maze, float world_x, float world_y) {
    int gx = (int)(world_x / TILE_SIZE);
    int gy = (int)(world_y / TILE_SIZE);
    if (gx < 0 || gx >= MAZE_COLS || gy < 0 || gy >= MAZE_ROWS)
        return T_WALL;
    return maze->data[gy][gx];
}

bool maze_is_portal_here(Maze *maze, float world_x, float world_y, int *out_idx) {
    int t = maze_tile_at(maze, world_x, world_y);
    if (t >= T_PORTAL1 && t <= T_PORTAL4) {
        if (out_idx) *out_idx = t - T_PORTAL1;
        return true;
    }
    return false;
}

void maze_unload(Maze *maze) {
    if (!maze) return;

    if (maze->wall_img)  { al_destroy_bitmap(maze->wall_img);  maze->wall_img  = NULL; }
    if (maze->floor_img) { al_destroy_bitmap(maze->floor_img); maze->floor_img = NULL; }

    for (int i = 0; i < 4; i++) {
        if (maze->portal_img[i]) {
            al_destroy_bitmap(maze->portal_img[i]);
            maze->portal_img[i] = NULL;
        }
    }
}
