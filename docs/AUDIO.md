# Audio System

BlankC Engine menggunakan library pihak ketiga **`miniaudio`** (sebagai *single-header library*) untuk menangani seluruh kebutuhan pemutaran suara. Modul ini dirancang agar sangat mudah digunakan untuk memutar efek suara (Sound Effects/SFX) maupun musik latar (Background Music/BGM).

## 1. Konsep Utama

Sistem audio (`audio.h`) dalam BlankC Engine membagi pemutaran suara menjadi dua kategori utama:

1.  **Sound Effects (SFX):** 
    - Digunakan untuk suara pendek seperti tembakan, langkah kaki, atau benturan.
    - **Overlapping:** Sistem ini mendukung pemutaran suara yang sama berkali-kali secara bersamaan tanpa saling memotong (overlap). Setiap kali dipanggil, engine akan mencari/membuat channel baru (fire-and-forget).
    - Format yang disarankan: `.wav`.
2.  **Background Music (BGM):** 
    - Digunakan untuk musik latar yang panjang.
    - **Streaming:** File tidak diload sepenuhnya ke RAM sekaligus, melainkan di-stream dari disk. Ini menghemat memori untuk file berukuran besar.
    - **Looping:** Secara default, musik akan diputar berulang-ulang (loop). Hanya ada satu BGM yang bisa diputar pada satu waktu. Jika BGM baru diputar, BGM lama akan otomatis dihentikan.
    - Format yang disarankan: `.mp3`.

## 2. Inisialisasi & Shutdown

Sistem audio dikelola langsung oleh struktur `Engine`. Anda tidak perlu menginisialisasinya secara manual jika menggunakan `engine_create()`.

Secara internal:
- `audio_init()`: Meminta akses ke perangkat audio OS dan menyiapkan *engine context* dari `miniaudio`.
- `audio_shutdown()`: Menghentikan semua suara, membersihkan *context*, dan melepaskan perangkat audio.

## 3. Menangani File Path (PENTING)

Salah satu penyebab utama audio gagal diputar adalah jalur file (*file path*) yang salah. BlankC Engine menyediakan fungsi pembantu `asset_path()` di modul `loader.c` untuk mengatasi hal ini.

Fungsi `asset_path` akan mencari lokasi absolut dari folder `assets/` relatif terhadap lokasi file *executable* dijalankan. **Penting:** Anda harus melakukan `free()` pada string yang dikembalikan oleh `asset_path()`.

**Contoh Penggunaan Benar:**
```c
char* snd_path = asset_path("assets/audio/jump.wav");
if (snd_path) {
    audio_play_sound(engine->audio, snd_path);
    free(snd_path); // WAJIB: Mencegah memory leak
}
```

## 4. Keterbatasan (Limitations)

- **Positional Audio (3D Sound):** Versi saat ini dari BlankC Engine **belum** mendukung audio posisional 3D (suara mengecil saat menjauh atau *panning* kiri/kanan berdasarkan posisi kamera). Semua suara diputar secara stereo/mono konvensional di tengah.
- **Volume Control:** Belum ada fungsi *wrapper* publik untuk mengatur volume global secara langsung via `audio.h` (meskipun `miniaudio` secara internal mendukungnya). Volume harus diatur melalui file asal aset tersebut.
