# Audio System

BlankC Engine uses the third-party library **`miniaudio`** (as a *single-header library*) to handle all sound playback needs. This module is designed to be very easy to use for playing Sound Effects (SFX) as well as Background Music (BGM).

## 1. Core Concepts

The audio system (`audio.h`) in BlankC Engine divides sound playback into two main categories:

1.  **Sound Effects (SFX):** 
    - Used for short sounds like gunshots, footsteps, or impacts.
    - **Overlapping:** This system supports playing the same sound multiple times simultaneously without cutting each other off (overlapping). Every time it's called, the engine will find/create a new channel (fire-and-forget).
    - Recommended format: `.wav`.
2.  **Background Music (BGM):** 
    - Used for long background tracks.
    - **Streaming:** The file is not fully loaded into RAM at once, but rather streamed from disk. This saves memory for large files.
    - **Looping:** By default, the music will loop continuously. Only one BGM track can be played at a time. If a new BGM is played, the old one will automatically stop.
    - Recommended format: `.mp3`.

## 2. Initialization & Shutdown

The audio system is managed directly by the `Engine` structure. You do not need to initialize it manually if you use `engine_create()`.

Internally:
- `audio_init()`: Requests access to the OS audio device and sets up the *engine context* from `miniaudio`.
- `audio_shutdown()`: Stops all sounds, cleans up the *context*, and releases the audio device.

## 3. Handling File Paths (IMPORTANT)

One of the main causes for audio failing to play is an incorrect *file path*. BlankC Engine provides a helper function `asset_path()` in the `loader.c` module to solve this.

The `asset_path` function will find the absolute location of the `assets/` folder relative to the location where the *executable* file is run. **Important:** You must call `free()` on the string returned by `asset_path()`.

**Correct Usage Example:**
```c
char* snd_path = asset_path("assets/audio/jump.wav");
if (snd_path) {
    audio_play_sound(engine->audio, snd_path);
    free(snd_path); // REQUIRED: Prevent memory leak
}
```

## 4. Limitations

- **Positional Audio (3D Sound):** The current version of BlankC Engine **does not** yet support positional 3D audio (sounds fading when moving away or *panning* left/right based on camera position). All sounds are played in conventional stereo/mono centered.
- **Volume Control:** There are currently no public *wrapper* functions to directly adjust global volume via `audio.h` (although `miniaudio` internally supports it). Volume should be adjusted via the source asset files for now.