#include "game.h"
#include "maze.h"
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "quiz.h"
#include "sound.h"

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_ENEMIES 9
#define MAX_LIVES 5

/* Mantém vidas dentro do limite */
static inline void clamp_lives(Player *p)
{
    if (p->lives < 0) p->lives = 0;
    if (p->lives > MAX_LIVES) p->lives = MAX_LIVES;
}

/* Texto inicial do pergaminho */
static void show_intro_scroll(ALLEGRO_FONT *font)
{
    ALLEGRO_BITMAP *scroll = al_load_bitmap("assets/ui/pergaminho.png");
    if (!scroll) return;

    sound_play(g_snd_scroll_open);

    const char *texto =
        "Saudacoes, viajante do tempo!\n\n"
        "Nos labirintos do passado, tu enfrentaras cavaleiros antigos.\n"
        "O dourado te persegue sem descanso;\n\n"
        "O vermelho atira de longe.\n\n"
        "Move-te com as setas, ataca com ESPACO, pausa com P.\n"
        "Multe a música com M.\n"
        "Boa sorte em tua jornada.\n\n";

    ALLEGRO_EVENT_QUEUE *q = al_create_event_queue();
    al_register_event_source(q, al_get_keyboard_event_source());

    int visible = 0;
    double start = al_get_time();
    bool skip = false;

    while (1)
    {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(q, &ev))
        {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN &&
                ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                skip = true;
                break;
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        int sw = al_get_bitmap_width(scroll);
        int sh = al_get_bitmap_height(scroll);
        int dw = al_get_display_width(al_get_current_display());
        int dh = al_get_display_height(al_get_current_display());

        al_draw_scaled_bitmap(scroll, 0, 0, sw, sh,
                              100, 60, dw - 200, dh - 120, 0);

        visible = (int)((al_get_time() - start) * 15);
        if (visible > strlen(texto))
            visible = strlen(texto);

        char buf[1024];
        strncpy(buf, texto, visible);
        buf[visible] = '\0';

        al_draw_multiline_text(font, al_map_rgb(40, 25, 10),
                               dw / 2, 250, 700, 28,
                               ALLEGRO_ALIGN_CENTER, buf);

        al_flip_display();
        al_rest(0.02);

        if (visible == (int)strlen(texto))
            break;
    }

    al_destroy_event_queue(q);
    al_destroy_bitmap(scroll);

    if (!skip)
        al_rest(0.25);
}

/* HUD com info básica */
static void draw_hud(ALLEGRO_FONT *font, ALLEGRO_BITMAP *hud,
                     int vidas, int fase, const char *nome)
{
    float w = 320, h = 32;

    if (hud)
    {
        al_draw_tinted_scaled_bitmap(
            hud, al_map_rgba_f(1, 1, 1, 0.3f),
            0, 0, al_get_bitmap_width(hud), al_get_bitmap_height(hud),
            10, 10, w, h, 0);
    }
    else
    {
        al_draw_filled_rectangle(10, 10, 10 + w, 10 + h,
                                 al_map_rgba(40, 30, 10, 180));
    }

    float ty = 10 + (h - al_get_font_line_height(font)) / 2;

    al_draw_textf(font, al_map_rgb(255, 230, 180),
                  20, ty, 0,
                  "Vidas: %d  Fase: %d  Jogador: %s",
                  vidas, fase, nome);
}

/* Inimigos espalhados */
static void spawn_enemies_for_level(Enemy e[MAX_ENEMIES], int level)
{
    const char *spr1 = "assets/sprites/knight_1.png";
    const char *spr2 = "assets/sprites/knight_2.png";

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        EnemyType t = (rand() % 2 == 0 ? ENEMY_CHASER : ENEMY_SHOOTER);
        const char *sp = (t == ENEMY_CHASER ? spr1 : spr2);

        int rx, ry;

        do {
            rx = (rand() % (MAZE_COLS - 4)) + 2;
            ry = (rand() % (MAZE_ROWS - 4)) + 2;
        } while (rx == 2 && ry == 2);

        enemy_init(&e[i], rx * TILE_SIZE, ry * TILE_SIZE, sp, t);
    }
}

/* Tela de game over */
static void show_game_over(ALLEGRO_FONT *font, const char *player_name)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(font, al_map_rgb(255, 50, 50),
                 640, 300, ALLEGRO_ALIGN_CENTER, "GAME OVER!");

    al_draw_textf(font, al_map_rgb(255, 255, 255),
                  640, 360, ALLEGRO_ALIGN_CENTER,
                  "Boa sorte na próxima, %s!", player_name);

    al_draw_text(font, al_map_rgb(180, 180, 180),
                 640, 420, ALLEGRO_ALIGN_CENTER,
                 "Pressione ENTER...");

    al_flip_display();

    ALLEGRO_EVENT_QUEUE *q = al_create_event_queue();
    al_register_event_source(q, al_get_keyboard_event_source());

    ALLEGRO_EVENT ev;
    while (1)
    {
        al_wait_for_event(q, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN &&
            ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            break;
    }

    al_destroy_event_queue(q);
}

/* GAME LOOP */

void game_loop(const char *avatar_path, const char *player_name)
{
    ALLEGRO_DISPLAY *disp = al_get_current_display();
    if (!disp) return;

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0f / FPS);

    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    ALLEGRO_FONT *font = al_load_font("assets/fonts/font.ttf", 20, 0);
    if (!font) font = al_create_builtin_font();

    ALLEGRO_FONT *font_controls = al_load_font("assets/fonts/font.ttf", 30, 0);
    if (!font_controls) font_controls = font;

    const char *spr = "assets/sprites/player_male.png";
    if (avatar_path && strstr(avatar_path, "female"))
        spr = "assets/sprites/player_female.png";

    Player player;
    player_init(&player, TILE_SIZE * 2, TILE_SIZE * 2, spr);
    player.speed = 2.4f;

    Maze maze;
    int level = 1;
    maze_load(&maze, level);

    Enemy enemies[MAX_ENEMIES];
    spawn_enemies_for_level(enemies, level);

    QuizBank qb;
    if (!load_quizzes(&qb, "assets/quizzes"))
        qb.count = 0;

    ALLEGRO_BITMAP *hud = al_load_bitmap("assets/ui/hud_wood.png");

    show_intro_scroll(font);

    bool running = true;
    bool paused = false;
    bool gotKillBonus = false;

    int meleeCooldown = 0;

    ALLEGRO_EVENT ev;

    while (running)
    {
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            return;

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_P)
                paused = !paused;

            if (ev.keyboard.keycode == ALLEGRO_KEY_M)
                sound_toggle_mute();

            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                return; /* voltar ao menu */
        }


        if (paused)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 120),
                         640, 360, ALLEGRO_ALIGN_CENTER,
                         "PAUSADO (P para voltar)");
            al_flip_display();
            continue;
        }


        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (meleeCooldown > 0)
                meleeCooldown--;

            /* Movimento */
            ALLEGRO_KEYBOARD_STATE ks;
            al_get_keyboard_state(&ks);
            player_update(&player, &maze, &ks);

            /* Inimigos */

            for (int i = 0; i < MAX_ENEMIES; i++)
                if (enemies[i].x >= 0)
                    enemy_update(&enemies[i], &maze, &player, level);

            /* Projéteis do player */
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].x < 0) continue;

                for (int j = 0; j < MAX_PROJECTILES; j++)
                {
                    Projectile *p = &player.projectiles[j];
                    if (!p->active) continue;

                    float dx = p->x - enemies[i].x;
                    float dy = p->y - enemies[i].y;

                    if (dx*dx + dy*dy < (TILE_SIZE*0.5f)*(TILE_SIZE*0.5f))
                    {
                        enemies[i].x = -9999;
                        enemies[i].y = -9999;
                        p->active = false;
                    }
                }
            }

            /* Projéteis inimigos */
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                for (int j = 0; j < MAX_PROJECTILES; j++)
                {
                    Projectile *p = &enemies[i].projectiles[j];
                    if (!p->active) continue;

                    float dx = p->x - player.x;
                    float dy = p->y - player.y;

                    if (dx*dx + dy*dy < (TILE_SIZE*0.4f)*(TILE_SIZE*0.4f))
                    {
                        p->active = false;
                        player.lives--;
                        clamp_lives(&player);
                        sound_play(g_snd_lose_life);

                        if (player.lives <= 0)
                            running = false;
                    }
                }
            }

            /* Dano corpo a corpo */
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].type != ENEMY_CHASER) continue;
                if (enemies[i].x < 0) continue;

                float dx = player.x - enemies[i].x;
                float dy = player.y - enemies[i].y;

                if (dx*dx + dy*dy < (TILE_SIZE*0.62f)*(TILE_SIZE*0.62f))
                {
                    if (meleeCooldown == 0)
                    {
                        player.lives -= 2;
                        clamp_lives(&player);

                        meleeCooldown = 45;
                        sound_play(g_snd_lose_life);

                        if (player.lives <= 0)
                            running = false;
                    }
                }
            }

            /* Matou todos */
            bool allDead = true;
            for (int i = 0; i < MAX_ENEMIES; i++)
                if (enemies[i].x > 0)
                    allDead = false;

            if (allDead && !gotKillBonus)
            {
                if (player.lives < MAX_LIVES)
                    player.lives++;

                clamp_lives(&player);
                gotKillBonus = true;
            }

            /* Portal / quiz */
            int px = player.x / TILE_SIZE;
            int py = player.y / TILE_SIZE;

            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    int gx = px + dx;
                    int gy = py + dy;

                    if (gx < 0 || gy < 0 || gx >= MAZE_COLS || gy >= MAZE_ROWS)
                        continue;

                    int tile = maze.data[gy][gx];

                    if (tile >= T_PORTAL1 && tile <= T_PORTAL4)
                    {
                        al_stop_timer(timer);

                        sound_play(g_snd_portal);

                        bool acertou = true;
                        if (qb.count > 0)
                            acertou = show_quiz_for_level(disp, &qb, level);

                        if (acertou)
                        {
                            /* vida do portal */
                            if (player.lives < MAX_LIVES)
                                player.lives++;

                            clamp_lives(&player);
                            level++;
                        }
                        else
                        {
                            if (level > 1)
                                level--;
                        }

                        if (level > 4)
                        {
                            running = false;
                        }
                        else
                        {
                            maze_unload(&maze);
                            maze_load(&maze, level);
                            spawn_enemies_for_level(enemies, level);

                            player.x = TILE_SIZE * 2;
                            player.y = TILE_SIZE * 2;

                            gotKillBonus = false;
                            meleeCooldown = 40;
                        }

                        al_start_timer(timer);
                    }
                }
            }

            /* Desenho */
            al_clear_to_color(al_map_rgb(0, 0, 0));

            maze_draw(&maze);
            player_draw(&player);

            for (int i = 0; i < MAX_ENEMIES; i++)
                if (enemies[i].x > 0)
                    enemy_draw(&enemies[i]);

            draw_hud(font, hud, player.lives, level, player_name);

            /* CONTROLES (lado direito, fonte grande)*/

            int fim_labirinto = MAZE_OFF_X + MAZE_COLS * MAZE_TILE_W;
            int espaço_preto = al_get_display_width(disp) - fim_labirinto;

            int cx = fim_labirinto + espaço_preto * 0.22;
            int cy = 150;

            ALLEGRO_COLOR ctrl_color = al_map_rgb(255, 255, 200);

            al_draw_text(font_controls, ctrl_color, cx, cy, 0, "CONTROLES");
            cy += 55;

            al_draw_text(font_controls, ctrl_color, cx, cy, 0, "   ↑");
            cy += 45;

            al_draw_text(font_controls, ctrl_color, cx, cy, 0, "← + →   Mov");
            cy += 45;

            al_draw_text(font_controls, ctrl_color, cx, cy, 0, "   ↓");
            cy += 65;

            al_draw_text(font_controls, ctrl_color, cx, cy, 0, "[ESP]  Atacar");
            cy += 55;

            al_draw_text(font_controls, ctrl_color, cx, cy, 0, "[P]    Pausa");
            cy += 55;

            al_draw_text(font_controls, ctrl_color, cx, cy, 0, "[M]    Mudo");

            al_flip_display();
        }
    }

    /* Morreu */
    if (player.lives <= 0)
        show_game_over(font, player_name);

    /* Liberações */
    free_quizzes(&qb);
    maze_unload(&maze);

    if (hud) al_destroy_bitmap(hud);
    if (font_controls && font_controls != font)
        al_destroy_font(font_controls);
    if (font) al_destroy_font(font);
    if (timer) al_destroy_timer(timer);
    if (queue) al_destroy_event_queue(queue);
}
