#include "sound.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdbool.h>

// Controle global de som (mute/desmute)
bool g_audio_muted = false;

// Amostras de som (arquivos carregados)
ALLEGRO_SAMPLE *g_snd_player_shoot = NULL;
ALLEGRO_SAMPLE *g_snd_enemy_shoot  = NULL;
ALLEGRO_SAMPLE *g_snd_portal       = NULL;
ALLEGRO_SAMPLE *g_snd_lose_life    = NULL;
ALLEGRO_SAMPLE *g_snd_scroll_open  = NULL;

// Instâncias de som (permitem tocar simultaneamente sem misturar)
static ALLEGRO_SAMPLE_INSTANCE *inst_player_shoot = NULL;
static ALLEGRO_SAMPLE_INSTANCE *inst_enemy_shoot  = NULL;
static ALLEGRO_SAMPLE_INSTANCE *inst_portal       = NULL;
static ALLEGRO_SAMPLE_INSTANCE *inst_lose_life    = NULL;
static ALLEGRO_SAMPLE_INSTANCE *inst_scroll_open  = NULL;

// Inicializa o sistema de áudio e carrega os sons do jogo
bool sound_init(void) {
    if (!al_is_audio_installed())
        al_install_audio();

    if (!al_init_acodec_addon()) {
        printf("Erro ao inicializar codecs de áudio.\n");
        return false;
    }

    al_reserve_samples(8);

    // Carrega os arquivos de som
    g_snd_player_shoot = al_load_sample("assets/audio/shoot.wav");
    g_snd_enemy_shoot  = al_load_sample("assets/audio/enemy_shoot.wav");
    g_snd_portal       = al_load_sample("assets/audio/portal.wav");
    g_snd_lose_life    = al_load_sample("assets/audio/lose_life.wav");
    g_snd_scroll_open  = al_load_sample("assets/audio/scroll_open.wav");

    if (!g_snd_player_shoot || !g_snd_enemy_shoot || !g_snd_portal || !g_snd_lose_life || !g_snd_scroll_open) {
        printf("Erro ao carregar um ou mais sons.\n");
        return false;
    }

    // Cria instâncias para cada som
    inst_player_shoot = al_create_sample_instance(g_snd_player_shoot);
    inst_enemy_shoot  = al_create_sample_instance(g_snd_enemy_shoot);
    inst_portal       = al_create_sample_instance(g_snd_portal);
    inst_lose_life    = al_create_sample_instance(g_snd_lose_life);
    inst_scroll_open  = al_create_sample_instance(g_snd_scroll_open);

    // Conecta todas as instâncias ao mixer padrão
    al_attach_sample_instance_to_mixer(inst_player_shoot, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(inst_enemy_shoot,  al_get_default_mixer());
    al_attach_sample_instance_to_mixer(inst_portal,       al_get_default_mixer());
    al_attach_sample_instance_to_mixer(inst_lose_life,    al_get_default_mixer());
    al_attach_sample_instance_to_mixer(inst_scroll_open,  al_get_default_mixer());

    // Define o modo de reprodução (somente uma vez)
    al_set_sample_instance_playmode(inst_player_shoot, ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(inst_enemy_shoot,  ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(inst_portal,       ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(inst_lose_life,    ALLEGRO_PLAYMODE_ONCE);
    al_set_sample_instance_playmode(inst_scroll_open,  ALLEGRO_PLAYMODE_ONCE);

    printf("Sistema de som inicializado com sucesso.\n");
    return true;
}

// Função genérica para tocar qualquer som do jogo
void sound_play(ALLEGRO_SAMPLE *sample) {
    if (g_audio_muted || !sample)
        return;

    ALLEGRO_SAMPLE_INSTANCE *inst = NULL;

    if (sample == g_snd_player_shoot) inst = inst_player_shoot;
    else if (sample == g_snd_enemy_shoot) inst = inst_enemy_shoot;
    else if (sample == g_snd_portal) inst = inst_portal;
    else if (sample == g_snd_lose_life) inst = inst_lose_life;
    else if (sample == g_snd_scroll_open) inst = inst_scroll_open;

    if (inst) {
        al_stop_sample_instance(inst);
        al_play_sample_instance(inst);
    }
}

// Alterna entre som ligado/desligado
void sound_toggle_mute(void) {
    g_audio_muted = !g_audio_muted;
    al_set_mixer_gain(al_get_default_mixer(), g_audio_muted ? 0.0 : 1.0);
    if (g_audio_muted)
        printf("Som desativado.\n");
    else
        printf("Som ativado.\n");
}

// Libera todos os recursos de áudio da memória
void sound_destroy(void) {
    if (inst_player_shoot) al_destroy_sample_instance(inst_player_shoot);
    if (inst_enemy_shoot)  al_destroy_sample_instance(inst_enemy_shoot);
    if (inst_portal)       al_destroy_sample_instance(inst_portal);
    if (inst_lose_life)    al_destroy_sample_instance(inst_lose_life);
    if (inst_scroll_open)  al_destroy_sample_instance(inst_scroll_open);

    if (g_snd_player_shoot) al_destroy_sample(g_snd_player_shoot);
    if (g_snd_enemy_shoot)  al_destroy_sample(g_snd_enemy_shoot);
    if (g_snd_portal)       al_destroy_sample(g_snd_portal);
    if (g_snd_lose_life)    al_destroy_sample(g_snd_lose_life);
    if (g_snd_scroll_open)  al_destroy_sample(g_snd_scroll_open);

    printf("Recursos de som liberados.\n");
}
