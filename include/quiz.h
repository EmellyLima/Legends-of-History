#ifndef QUIZ_H
#define QUIZ_H

#include <allegro5/allegro.h>
#include <stdbool.h>

#define MAX_ANSWERS 4
#define MAX_QUIZZES 10

// Estrutura de uma pergunta
typedef struct {
    char question[256];            // Texto da pergunta
    char answers[MAX_ANSWERS][128]; // Alternativas
    int correct_index;             // Índice da resposta correta
    int level;                     // Nível de dificuldade (1 a 4)
} Quiz;

// Estrutura que armazena o conjunto de perguntas
typedef struct {
    Quiz quizzes[MAX_QUIZZES];
    int count;
} QuizBank;

// Inicializa o banco de perguntas
bool load_quizzes(QuizBank *qb, const char *folder);

// Exibe uma pergunta de acordo com o nível (fase)
// Retorna true se o jogador acertar
bool show_quiz_for_level(ALLEGRO_DISPLAY *display, QuizBank *qb, int level);

// Libera o banco de perguntas
void free_quizzes(QuizBank *qb);

#endif
