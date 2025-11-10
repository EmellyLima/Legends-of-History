#include "quiz.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Inicializa o banco de perguntas (1 por fase, 4 fases)
bool load_quizzes(QuizBank *qb, const char *folder) {
    (void)folder;
    qb->count = 0;

    // ----- Fase 1 (Fácil) -----
    strcpy(qb->quizzes[qb->count].question, "Quem foi o primeiro imperador romano?");
    strcpy(qb->quizzes[qb->count].answers[0], "Nero");
    strcpy(qb->quizzes[qb->count].answers[1], "Júlio César");
    strcpy(qb->quizzes[qb->count].answers[2], "Augusto");
    strcpy(qb->quizzes[qb->count].answers[3], "Trajano");
    qb->quizzes[qb->count].correct_index = 2;
    qb->quizzes[qb->count++].level = 1;

    // ----- Fase 2 (Médio) -----
    strcpy(qb->quizzes[qb->count].question, "Em que ano ocorreu a Independência do Brasil?");
    strcpy(qb->quizzes[qb->count].answers[0], "1822");
    strcpy(qb->quizzes[qb->count].answers[1], "1889");
    strcpy(qb->quizzes[qb->count].answers[2], "1808");
    strcpy(qb->quizzes[qb->count].answers[3], "1831");
    qb->quizzes[qb->count].correct_index = 0;
    qb->quizzes[qb->count++].level = 2;

    // ----- Fase 3 (Difícil) -----
    strcpy(qb->quizzes[qb->count].question, "Quem foi o líder militar responsável pela conquista do Império Inca?");
    strcpy(qb->quizzes[qb->count].answers[0], "Hernán Cortés");
    strcpy(qb->quizzes[qb->count].answers[1], "Francisco Pizarro");
    strcpy(qb->quizzes[qb->count].answers[2], "Cristóvão Colombo");
    strcpy(qb->quizzes[qb->count].answers[3], "Pedro Álvares Cabral");
    qb->quizzes[qb->count].correct_index = 1;
    qb->quizzes[qb->count++].level = 3;

    // ----- Fase 4 (Extremamente Difícil) -----
    strcpy(qb->quizzes[qb->count].question, "Qual foi o tratado que encerrou oficialmente a Primeira Guerra Mundial?");
    strcpy(qb->quizzes[qb->count].answers[0], "Tratado de Versalhes");
    strcpy(qb->quizzes[qb->count].answers[1], "Tratado de Tordesilhas");
    strcpy(qb->quizzes[qb->count].answers[2], "Tratado de Paris");
    strcpy(qb->quizzes[qb->count].answers[3], "Tratado de Viena");
    qb->quizzes[qb->count].correct_index = 0;
    qb->quizzes[qb->count++].level = 4;

    return (qb->count > 0);
}

// Exibe pergunta correspondente à fase atual
bool show_quiz_for_level(ALLEGRO_DISPLAY *display, QuizBank *qb, int level) {
    if (!qb || qb->count == 0 || !display)
        return true;

    Quiz *q = NULL;
    for (int i = 0; i < qb->count; i++) {
        if (qb->quizzes[i].level == level) {
            q = &qb->quizzes[i];
            break;
        }
    }
    if (!q) return true; // se não encontrar, passa direto

    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_FONT *font_q = al_load_font("assets/fonts/MedievalSharp.ttf", 30, 0);
    ALLEGRO_FONT *font_a = al_load_font("assets/fonts/MedievalSharp.ttf", 24, 0);
    if (!font_q) font_q = al_create_builtin_font();
    if (!font_a) font_a = al_create_builtin_font();

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    int selected = 0;
    bool done = false;
    bool correct = false;

    while (!done) {
        al_clear_to_color(al_map_rgb(15, 10, 15));

        al_draw_multiline_text(font_q, al_map_rgb(255, 220, 100),
                               al_get_display_width(display) / 2, 100, 1000,
                               40, ALLEGRO_ALIGN_CENTER, q->question);

        for (int i = 0; i < 4; i++) {
            ALLEGRO_COLOR color = (i == selected)
                ? al_map_rgb(255, 80, 80)
                : al_map_rgb(230, 230, 230);
            al_draw_textf(font_a, color,
                          al_get_display_width(display) / 2, 260 + i * 60,
                          ALLEGRO_ALIGN_CENTER, "%d) %s", i + 1, q->answers[i]);
        }

        al_draw_text(font_a, al_map_rgb(200, 200, 200),
                     al_get_display_width(display) / 2,
                     al_get_display_height(display) - 80,
                     ALLEGRO_ALIGN_CENTER,
                     "↑ ↓ para mover  |  ENTER para confirmar");

        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    selected = (selected + 3) % 4;
                    break;
                case ALLEGRO_KEY_DOWN:
                    selected = (selected + 1) % 4;
                    break;
                case ALLEGRO_KEY_ENTER:
                    done = true;
                    correct = (selected == q->correct_index);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
            }
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
    }

    // Resultado
    al_clear_to_color(correct ? al_map_rgb(0, 100, 0) : al_map_rgb(120, 0, 0));
    ALLEGRO_FONT *font_r = al_create_builtin_font();
    al_draw_text(font_r, al_map_rgb(255, 255, 255),
                 al_get_display_width(display) / 2,
                 al_get_display_height(display) / 2,
                 ALLEGRO_ALIGN_CENTER,
                 correct ? "Resposta Correta!" : "Resposta Errada!");
    al_draw_text(font_r, al_map_rgb(220, 220, 220),
                 al_get_display_width(display) / 2,
                 al_get_display_height(display) / 2 + 40,
                 ALLEGRO_ALIGN_CENTER, "Pressione ENTER para continuar...");
    al_flip_display();

    // Espera confirmação
    ALLEGRO_EVENT_QUEUE *wait_q = al_create_event_queue();
    al_register_event_source(wait_q, al_get_keyboard_event_source());
    bool waiting = true;
    while (waiting) {
        ALLEGRO_EVENT e;
        al_wait_for_event(wait_q, &e);
        if (e.type == ALLEGRO_EVENT_KEY_DOWN && e.keyboard.keycode == ALLEGRO_KEY_ENTER)
            waiting = false;
    }

    al_destroy_event_queue(wait_q);
    al_destroy_event_queue(queue);
    al_destroy_font(font_q);
    al_destroy_font(font_a);
    al_destroy_font(font_r);

    return correct;
}

// Limpa o banco de quizzes
void free_quizzes(QuizBank *qb) {
    if (!qb) return;
    qb->count = 0;
}
