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

/* Mantém valor das vidas dentro do intervalo permitido */
static inline void clamp_lives(Player *p)
{
    if (p->lives < 0)
        p->lives = 0;
    if (p->lives > MAX_LIVES)
        p->lives = MAX_LIVES;
}

/* Tela inicial com texto em pergaminho */
static void show_intro_scroll(ALLEGRO_FONT *font)
{
    ALLEGRO_BITMAP *scroll = al_load_bitmap("assets/ui/pergaminho.png");
    if (!scroll)
        return;

    sound_play(g_snd_scroll_open);

    const char *texto =
        "Saudacoes, viajante do tempo!\n\n"
        "Nos labirintos de Legends of History, tua missao e alcancar\n"
        "o portal sagrado com o maximo de vidas possivel.\n"
        "Um cavaleiro dourado te persegue de perto e causa grande dano.\n"
        "Um cavaleiro vermelho ataca a distancia com projeteis.\n\n"
        "Setas: mover | Espaco: atacar | P: pausar | M: mutar o som\n\n";

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());

    double start = al_get_time();
    int visible = 0;
    bool skip = false;

    while (true)
    {
        ALLEGRO_EVENT e;
        if (al_get_next_event(queue, &e))
        {
            if (e.type == ALLEGRO_EVENT_KEY_DOWN &&
                e.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                skip = true;
                break;
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        int dw = al_get_display_width(al_get_current_display());
        int dh = al_get_display_height(al_get_current_display());
        int sw = al_get_bitmap_width(scroll);
        int sh = al_get_bitmap_height(scroll);

        al_draw_scaled_bitmap(scroll, 0, 0, sw, sh,
                              100, 60, dw - 200, dh - 120, 0);

        visible = (int)((al_get_time() - start) * 15);
        if (visible > (int)strlen(texto))
            visible = strlen(texto);

        char temp[1024];
        strncpy(temp, texto, visible);
        temp[visible] = '\0';

        al_draw_multiline_text(font, al_map_rgb(40, 25, 10),
                               dw / 2, 250, 700, 28,
                               ALLEGRO_ALIGN_CENTER, temp);

        al_flip_display();
        al_rest(0.02);

        if (visible == (int)strlen(texto))
            break;
    }

    al_destroy_bitmap(scroll);
    al_destroy_event_queue(queue);

    if (!skip)
        al_rest(0.2);
}

/* HUD com informações básicas */
static void draw_hud(ALLEGRO_FONT *font, ALLEGRO_BITMAP *hud,
                     int vidas, int fase, const char *jogador)
{
    float width = 320, height = 32;

    if (hud)
    {
        al_draw_tinted_scaled_bitmap(
            hud, al_map_rgba_f(1, 1, 1, 0.3f), 0, 0,
            al_get_bitmap_width(hud),
            al_get_bitmap_height(hud),
            10, 10, width, height, 0);
    }
    else
    {
        al_draw_filled_rectangle(10, 10, 10 + width, 10 + height,
                                 al_map_rgba(40, 30, 10, 180));
    }

    float ty = 10 + (height - al_get_font_line_height(font)) * 0.5f;
    al_draw_textf(font, al_map_rgb(255, 230, 180), 20, ty, 0,
                  "Vidas: %d  Fase: %d  Jogador: %s",
                  vidas, fase, jogador);
}

static void spawn_enemies_for_level(Enemy enemies[MAX_ENEMIES], int level)
{
    const char *chaserSprite  = "assets/sprites/knight_1.png";
    const char *shooterSprite = "assets/sprites/knight_2.png";

    float boost = 1.0f + (level - 1) * 0.25f;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        /* Sorteia tipo do inimigo */
        int r = rand() % 2; 
        EnemyType type;
        const char *sprite;

        if (r == 0)
        {
            type = ENEMY_CHASER;     /* come-come */
            sprite = chaserSprite;
        }
        else
        {
            type = ENEMY_SHOOTER;    /* atirador */
            sprite = shooterSprite;
        }

        /* Posição aleatória válida dentro do labirinto */
        int rx = (rand() % (MAZE_COLS - 4)) + 2;
        int ry = (rand() % (MAZE_ROWS - 4)) + 2;

        enemy_init(&enemies[i],
                   rx * TILE_SIZE,
                   ry * TILE_SIZE,
                   sprite,
                   type);

        enemies[i].speed *= boost;
    }
}

/* Tela final (morte) */
static void show_game_over(ALLEGRO_FONT *font, const char *player_name)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(font, al_map_rgb(255, 50, 50),
                 640, 300, ALLEGRO_ALIGN_CENTER, "GAME OVER!");
    al_draw_textf(font, al_map_rgb(255, 255, 255),
                  640, 360, ALLEGRO_ALIGN_CENTER,
                  "Boa sorte na proxima, %s!", player_name);

    al_draw_text(font, al_map_rgb(180, 180, 180),
                 640, 420, ALLEGRO_ALIGN_CENTER,
                 "Pressione ENTER para sair...");

    al_flip_display();

    ALLEGRO_EVENT_QUEUE *q = al_create_event_queue();
    al_register_event_source(q, al_get_keyboard_event_source());

    while (true)
    {
        ALLEGRO_EVENT e;
        al_wait_for_event(q, &e);
        if (e.type == ALLEGRO_EVENT_KEY_DOWN &&
            e.keyboard.keycode == ALLEGRO_KEY_ENTER)
            break;
    }

    al_destroy_event_queue(q);
}
void game_loop(const char *avatar_path, const char *player_name)
{
    ALLEGRO_DISPLAY *display = al_get_current_display();
    if (!display)
        return;

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    ALLEGRO_FONT *font = al_load_font("assets/fonts/font.ttf", 20, 0);
    if (!font)
        font = al_create_builtin_font();

    /* Define sprite do jogador */
    const char *sprite_path = "assets/sprites/player_male.png";
    if (avatar_path && strstr(avatar_path, "female"))
        sprite_path = "assets/sprites/player_female.png";

    Player player;
    player_init(&player, TILE_SIZE * 2, TILE_SIZE * 2, sprite_path);
    player.lives = 3;

    Maze maze;
    int current_level = 1;
    maze_load(&maze, current_level);

    Enemy enemies[MAX_ENEMIES];
    spawn_enemies_for_level(enemies, current_level);

    QuizBank qb;
    if (!load_quizzes(&qb, "assets/quizzes"))
        qb.count = 0;

    show_intro_scroll(font);

    ALLEGRO_BITMAP *hud_texture = al_load_bitmap("assets/ui/hud_wood.png");

    bool running = true;
    bool paused = false;

    /* Controle de bônus por fase */
    bool bonusGivenThisLevel = false;
    bool quizBonusAllowed = false;

    int meleeCooldown = 0;

    ALLEGRO_EVENT ev;

    while (running)
    {
        al_wait_for_event(queue, &ev);

        /* Fechar pelo X */
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

        /* Pressionar tecla */
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_P)
                paused = !paused;

            else if (ev.keyboard.keycode == ALLEGRO_KEY_M)
                sound_toggle_mute();

            else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                /* Sai do jogo completamente */
                return;
            }
        }

        /* Tela de pausa */
        if (paused)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 120),
                         640, 360, ALLEGRO_ALIGN_CENTER,
                         "JOGO PAUSADO (P para continuar)");
            al_flip_display();
            continue;
        }

        /* Atualizações por timer */
        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (meleeCooldown > 0)
                meleeCooldown--;

            ALLEGRO_KEYBOARD_STATE key_state;
            al_get_keyboard_state(&key_state);
            player_update(&player, &maze, &key_state);

            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].x >= 0)
                    enemy_update(&enemies[i], &maze, &player, current_level);
            }

            /* Projéteis do player acertam inimigos */
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].x < 0)
                    continue;

                for (int j = 0; j < MAX_PROJECTILES; j++)
                {
                    Projectile *p = &player.projectiles[j];
                    if (!p->active)
                        continue;

                    float dx = p->x - enemies[i].x;
                    float dy = p->y - enemies[i].y;

                    if ((dx * dx + dy * dy) < (TILE_SIZE * 0.5f) * (TILE_SIZE * 0.5f))
                    {
                        enemies[i].x = -9999;
                        enemies[i].y = -9999;
                        p->active = false;
                    }
                }
            }

            /* Projéteis inimigos acertam jogador */
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                for (int j = 0; j < MAX_PROJECTILES; j++)
                {
                    Projectile *p = &enemies[i].projectiles[j];
                    if (!p->active)
                        continue;

                    float dx = p->x - player.x;
                    float dy = p->y - player.y;

                    if ((dx * dx + dy * dy) < (TILE_SIZE * 0.4f) * (TILE_SIZE * 0.4f))
                    {
                        p->active = false;
                        player.lives -= 1;
                        clamp_lives(&player);
                        sound_play(g_snd_lose_life);

                        if (player.lives <= 0)
                        {
                            running = false;
                            break;
                        }
                    }
                }
            }
            /* Dano corpo a corpo do chaser (com cooldown) */
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].type == ENEMY_CHASER && enemies[i].x > 0)
                {
                    float dx = player.x - enemies[i].x;
                    float dy = player.y - enemies[i].y;

                    if ((dx * dx + dy * dy) < (TILE_SIZE * 0.6f) * (TILE_SIZE * 0.6f))
                    {
                        if (meleeCooldown == 0)
                        {
                            player.lives -= 2;
                            clamp_lives(&player);
                            meleeCooldown = 45;
                            sound_play(g_snd_lose_life);

                            if (player.lives <= 0)
                            {
                                running = false;
                                break;
                            }
                        }
                    }
                }
            }

            /* Verifica se todos inimigos morreram */
            bool allDead = true;
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].x > 0)
                {
                    allDead = false;
                    break;
                }
            }

            /* Bonus de matar todos (1 por fase) */
            if (allDead && !bonusGivenThisLevel)
            {
                if (player.lives < MAX_LIVES)
                    player.lives++;

                clamp_lives(&player);
                bonusGivenThisLevel = true;
            }

            /* Portal e quiz */
            int px = (int)(player.x / TILE_SIZE);
            int py = (int)(player.y / TILE_SIZE);

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
                            acertou = show_quiz_for_level(display, &qb, current_level);

                        /* Se acertou, pode receber bonus da fase */
                        if (acertou)
                        {
                            quizBonusAllowed = true;
                            current_level++;
                        }
                        else
                        {
                            quizBonusAllowed = false;
                            if (current_level > 1)
                                current_level--;
                        }

                        /* Final do jogo */
                        if (current_level > 4)
                        {
                            running = false;
                        }
                        else
                        {
                            maze_unload(&maze);
                            maze_load(&maze, current_level);
                            spawn_enemies_for_level(enemies, current_level);

                            player.x = TILE_SIZE * 2;
                            player.y = TILE_SIZE * 2;

                            /* Bonus por passar de fase:
                               só se acertou o quiz E
                               não ganhou bonus matando todos */
                            if (quizBonusAllowed && !bonusGivenThisLevel)
                            {
                                if (player.lives < MAX_LIVES)
                                    player.lives++;

                                clamp_lives(&player);
                            }

                            /* reseta bonus para próxima fase */
                            bonusGivenThisLevel = false;
                            quizBonusAllowed = false;

                            meleeCooldown = 40;
                        }

                        al_start_timer(timer);
                    }
                }
            }

            /* Desenho do jogo */
            al_clear_to_color(al_map_rgb(0, 0, 0));
            maze_draw(&maze);
            player_draw(&player);

            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].x > 0)
                    enemy_draw(&enemies[i]);
            }

            draw_hud(font, hud_texture, player.lives, current_level, player_name);
            al_flip_display();
        }
    }

    /* Tela final caso morra */
    if (player.lives <= 0)
        show_game_over(font, player_name);
    /* Liberação de recursos */
    free_quizzes(&qb);
    maze_unload(&maze);

    if (hud_texture)
        al_destroy_bitmap(hud_texture);

    if (font)
        al_destroy_font(font);

    if (timer)
        al_destroy_timer(timer);

    if (queue)
        al_destroy_event_queue(queue);
}
/* Fim do arquivo game.c */
