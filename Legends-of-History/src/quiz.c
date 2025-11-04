// src/quiz.c
#define _POSIX_C_SOURCE 200809L
#include "quiz.h"
#include "config.h"            
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>            
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>


static char *strdup_n(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char *d = malloc(n+1);
    if (!d) return NULL;
    memcpy(d, s, n+1);
    return d;
}

bool load_quizzes(QuizBank *qb, const char *dirpath) {
    qb->items = NULL;
    qb->count = 0;

    DIR *d = opendir(dirpath);
    if (!d) return false;

    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] != '.') {
            // build full path
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
            FILE *f = fopen(path, "r");
            if (!f) continue;
            char *line = NULL;
            size_t len = 0;
            while (getline(&line, &len, f) != -1) {
                // trim newline
                while (line[strlen(line)-1]=='\n' || line[strlen(line)-1]=='\r') line[strlen(line)-1]=0;
                if (strlen(line) == 0) continue;
                // parse by '|'
                char *parts[6] = {0};
                int idx = 0;
                char *token = strtok(line, "|");
                while (token && idx < 6) {
                    parts[idx++] = token;
                    token = strtok(NULL, "|");
                }
                if (idx == 6) {
                    qb->items = realloc(qb->items, sizeof(QuizItem) * (qb->count + 1));
                    QuizItem *it = &qb->items[qb->count];
                    it->question = strdup_n(parts[0]);
                    for (int i=0;i<4;i++) it->options[i] = strdup_n(parts[1+i]);
                    it->correct = atoi(parts[5]); // assume 1..4
                    qb->count++;
                }
            }
            free(line);
            fclose(f);
        }
    }
    closedir(d);
    if (qb->count == 0) return false;
    srand((unsigned)time(NULL));
    return true;
}

void free_quizzes(QuizBank *qb) {
    if (!qb || !qb->items) return;
    for (int i=0;i<qb->count;i++) {
        free(qb->items[i].question);
        for (int j=0;j<4;j++) free(qb->items[i].options[j]);
    }
    free(qb->items);
    qb->items = NULL;
    qb->count = 0;
}

bool show_random_quiz(ALLEGRO_DISPLAY *display, QuizBank *qb) {
    if (!qb || qb->count == 0) return false;
    int id = rand() % qb->count;
    QuizItem *it = &qb->items[id];

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());

    // draw once
    ALLEGRO_FONT *font = al_create_builtin_font();

    bool answered = false;
    bool correct = false;

    while (!answered) {
        al_clear_to_color(al_map_rgb(10,10,30));

        if (font) {
            al_draw_text(font, al_map_rgb(255,255,255), 40, 50, 0, it->question);
            for (int i=0;i<4;i++) {
                char optline[512];
                snprintf(optline, sizeof(optline), "%d) %s", i+1, it->options[i]);
                al_draw_text(font, al_map_rgb(200,200,200), 60, 120 + i*40, 0, optline);
            }
            al_draw_text(font, al_map_rgb(180,180,180), 40, 320, 0, "Pressione 1-4 para escolher a resposta");
        }

        al_flip_display();

        // wait key
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            int k = ev.keyboard.keycode;
            int choice = 0;
            if (k == ALLEGRO_KEY_1) choice = 1;
            else if (k == ALLEGRO_KEY_2) choice = 2;
            else if (k == ALLEGRO_KEY_3) choice = 3;
            else if (k == ALLEGRO_KEY_4) choice = 4;

            if (choice >=1 && choice <=4) {
                answered = true;
                correct = (choice == it->correct);
            } else if (k == ALLEGRO_KEY_ESCAPE) {
                answered = true;
                correct = false;
            }
        }
    }

    al_destroy_event_queue(queue);
    ALLEGRO_FONT *font2 = al_create_builtin_font();
    if (font2) {
        al_clear_to_color(al_map_rgb(0,0,0));
        if (correct) al_draw_text(font2, al_map_rgb(0,255,0), SCREEN_WIDTH/2, SCREEN_HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "Correto!");
        else al_draw_text(font2, al_map_rgb(255,0,0), SCREEN_WIDTH/2, SCREEN_HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "Incorreto!");
        al_draw_text(font2, al_map_rgb(200,200,200), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 40, ALLEGRO_ALIGN_CENTRE, "Pressione qualquer tecla...");
        al_flip_display();
        ALLEGRO_EVENT_QUEUE *q2 = al_create_event_queue();
        al_register_event_source(q2, al_get_keyboard_event_source());
        ALLEGRO_EVENT ev2;
        al_wait_for_event(q2, &ev2);
        al_destroy_event_queue(q2);
    }
    return correct;
}

