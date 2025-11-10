#ifndef SOUND_H
#define SOUND_H

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>

/**
 * Inicializa o sistema de áudio (som e música).
 * Deve ser chamado uma vez no início do jogo.
 */
bool sound_init();

/**
 * Carrega um som curto (efeito) — WAV, OGG, MP3.
 */
ALLEGRO_SAMPLE* load_sound(const char* path);

/**
 * Toca um som curto (efeito) uma vez.
 */
void play_sound(ALLEGRO_SAMPLE* sample);

/**
 * Carrega e toca uma música de fundo em loop.
 * Retorna o ponteiro do sample tocando.
 */
ALLEGRO_SAMPLE* play_music_loop(const char* path, float volume);

/**
 * Para a música atual.
 */
void stop_music(ALLEGRO_SAMPLE* music);

/**
 * Finaliza o sistema de som (libera memória).
 */
void sound_shutdown();

#endif
