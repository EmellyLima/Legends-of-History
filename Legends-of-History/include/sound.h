#ifndef SOUND_H
#define SOUND_H

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

ALLEGRO_SAMPLE* load_sound(const char* path);

#endif
