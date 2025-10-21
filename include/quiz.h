#ifndef QUIZ_H
#define QUIZ_H

#include <allegro5/allegro.h>
#include <stdbool.h>

typedef struct {
    char *question;
    char *options[4];
    int correct; 
} QuizItem;

typedef struct {
    QuizItem *items;
    int count;
} QuizBank;

bool load_quizzes(QuizBank *qb, const char *dirpath);
void free_quizzes(QuizBank *qb);

// mostra uma questão aleatória; retorna true se acertou
bool show_random_quiz(ALLEGRO_DISPLAY *display, QuizBank *qb);

#endif

