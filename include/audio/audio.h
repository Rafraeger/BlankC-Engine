#ifndef AUDIO_H
#define AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AudioSystem AudioSystem;

AudioSystem* audio_init(void);
void audio_shutdown(AudioSystem* audio);

// Play a one-shot sound effect
void audio_play_sound(AudioSystem* audio, const char* filepath);

// Play background music (looping)
void audio_play_music(AudioSystem* audio, const char* filepath);

// Stop current background music
void audio_stop_music(AudioSystem* audio);

#ifdef __cplusplus
}
#endif

#endif /* AUDIO_H */
