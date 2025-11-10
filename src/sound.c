#include "sound.h"
#include <stdio.h>

static bool sound_ready = false;

bool sound_init() {
    if (!al_is_audio_installed())
        al_install_audio();
    if (!al_is_acodec_addon_initialized())
        al_init_acodec_addon();

    al_reserve_samples(16);
    sound_ready = true;
    printf("[SOUND] Sistema de áudio inicializado.\n");
    return true;
}

ALLEGRO_SAMPLE* load_sound(const char* path) {
    if (!sound_ready) sound_init();

    ALLEGRO_SAMPLE* s = al_load_sample(path);
    if (!s) {
        fprintf(stderr, "[SOUND] Falha ao carregar: %s\n", path);
    } else {
        printf("[SOUND] Carregado: %s\n", path);
    }
    return s;
}

void play_sound(ALLEGRO_SAMPLE* sample) {
    if (sample)
        al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

ALLEGRO_SAMPLE* play_music_loop(const char* path, float volume) {
    if (!sound_ready) sound_init();

    ALLEGRO_SAMPLE* music = al_load_sample(path);
    if (!music) {
        fprintf(stderr, "[SOUND] Falha ao carregar música: %s\n", path);
        return NULL;
    }
    al_play_sample(music, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    printf("[SOUND] Música tocando: %s\n", path);
    return music;
}

void stop_music(ALLEGRO_SAMPLE* music) {
    if (music) al_destroy_sample(music);
}

void sound_shutdown() {
    al_uninstall_audio();
    sound_ready = false;
}
