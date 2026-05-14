#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "audio/audio.h"
#include "core/log.h"
#include <stdlib.h>

struct AudioSystem {
    ma_engine engine;
    ma_sound bgm;
    int bgm_playing;
};

AudioSystem* audio_init(void) {
    AudioSystem* audio = malloc(sizeof(AudioSystem));
    if (!audio) return NULL;

    ma_result result = ma_engine_init(NULL, &audio->engine);
    if (result != MA_SUCCESS) {
        log_error("Failed to initialize audio engine.");
        free(audio);
        return NULL;
    }

    audio->bgm_playing = 0;
    log_info("Audio system initialized.");
    return audio;
}

void audio_shutdown(AudioSystem* audio) {
    if (audio) {
        if (audio->bgm_playing) {
            ma_sound_uninit(&audio->bgm);
        }
        ma_engine_uninit(&audio->engine);
        free(audio);
        log_info("Audio system shutdown.");
    }
}

void audio_play_sound(AudioSystem* audio, const char* filepath) {
    if (!audio) return;
    ma_result result = ma_engine_play_sound(&audio->engine, filepath, NULL);
    if (result != MA_SUCCESS) {
        log_warn("Failed to play sound: %s (result %d)", filepath, (int)result);
    }
}

void audio_play_music(AudioSystem* audio, const char* filepath) {
    if (!audio) return;

    if (audio->bgm_playing) {
        audio_stop_music(audio);
    }

    ma_result result = ma_sound_init_from_file(&audio->engine, filepath, MA_SOUND_FLAG_STREAM, NULL, NULL, &audio->bgm);
    if (result != MA_SUCCESS) {
        log_warn("Failed to load music: %s", filepath);
        return;
    }

    ma_sound_set_looping(&audio->bgm, MA_TRUE);
    ma_sound_start(&audio->bgm);
    audio->bgm_playing = 1;
    log_info("Playing music: %s", filepath);
}

void audio_stop_music(AudioSystem* audio) {
    if (audio && audio->bgm_playing) {
        ma_sound_stop(&audio->bgm);
        ma_sound_uninit(&audio->bgm);
        audio->bgm_playing = 0;
    }
}
