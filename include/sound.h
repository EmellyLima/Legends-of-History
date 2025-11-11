#ifndef SOUND_H
#define SOUND_H

#include <allegro5/allegro_audio.h>
#include <stdbool.h>

// -------------------------------------------------------
// 🎧 SISTEMA DE SOM - Legends of History
// -------------------------------------------------------
// Este módulo controla:
//  • Efeitos sonoros (tiros, portal, dano)
//  • Música de fundo (opcional)
//  • Controle de mute global
// -------------------------------------------------------

// Controle global de som (true = mutado)
extern bool g_audio_muted;

// Sons principais
extern ALLEGRO_SAMPLE *g_snd_player_shoot;
extern ALLEGRO_SAMPLE *g_snd_enemy_shoot;
extern ALLEGRO_SAMPLE *g_snd_portal;
extern ALLEGRO_SAMPLE *g_snd_lose_life;
extern ALLEGRO_SAMPLE *g_snd_scroll_open;

// Inicializa o sistema de áudio e carrega todos os sons.
// Retorna true se o carregamento for bem-sucedido.
bool sound_init(void);

// Toca um som (exemplo: sound_play(g_snd_player_shoot));
void sound_play(ALLEGRO_SAMPLE *sample);

// Liga/desliga o som (usado no jogo com a tecla 'M').
void sound_toggle_mute(void);

// Libera todos os sons e instâncias de áudio.
// Deve ser chamada ao sair do jogo.
void sound_destroy(void);

// --- Funções opcionais para música de fundo ---
void sound_play_music(void);
void sound_stop_music(void);

#endif // SOUND_H

