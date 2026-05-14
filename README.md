# BlankC Engine

## 1. Introduction

**BlankC Engine** adalah sebuah framework game 3D berbasis bahasa C. Engine ini dirancang dari bawah ke atas (from scratch) untuk memberikan fondasi yang kuat bagi developer yang ingin membangun game atau aplikasi interaktif 3D tanpa overhead dari game engine raksasa. 

Tujuan utama dari engine ini adalah menyediakan kerangka kerja yang solid untuk rendering, interaksi, fisika dasar, dan audio, sambil tetap memberikan kontrol penuh kepada programmer atas *main loop* dan manajemen memori.

## 2. Engine Philosophy

BlankC Engine dibangun dengan memegang teguh beberapa filosofi inti:

*   **Code-First:** Tidak ada visual editor (GUI). Game logika, penempatan objek, dan pengaturan scene murni ditulis menggunakan kode C.
*   **Lightweight & Minimal Abstraction:** Engine ini sangat ringan. Lapisan abstraksinya dibuat setipis mungkin agar developer mengerti persis apa yang terjadi di balik layar (tidak ada *magic*).
*   **Low-Level Control:** Programmer memiliki kontrol penuh terhadap siklus hidup aplikasi (*main loop*), alokasi memori (via custom Arena/Pool allocators), dan urutan eksekusi sistem (physics, update, render).
*   **Programmer-Oriented:** Dirancang khusus untuk programmer yang menyukai kendali penuh dan ingin belajar bagaimana sebuah game engine bekerja di tingkat dasar.

## 3. Features

Meskipun ringan, BlankC Engine telah dilengkapi dengan fitur-fitur esensial:

*   **Renderer:** Forward rendering pipeline menggunakan OpenGL 3.3 Core Profile. Mendukung custom shader, texturing, dan depth testing.
*   **Audio System:** Playback audio yang stabil untuk sound effect (WAV) dan background music (MP3) dengan dukungan overlapping, ditenagai oleh `miniaudio`.
*   **Physics & Collision:** Sistem fisika dasar dengan Dynamic RigidBody, deteksi tabrakan AABB (Axis-Aligned Bounding Box), dan resolusi tumbukan (MTV).
*   **Interaction & Raycasting:** Dukungan raycasting dari kamera (presisi tinggi) untuk interaksi objek di dunia 3D.
*   **Scene System:** Manajemen scene berbasis hirarki (Scene Graph) dengan sistem Transform (Position, Rotation, Scale) pewarisan parent-child.
*   **Asset Loader:** Pemuatan model 3D (`.obj`) dan tekstur secara efisien. Mendukung alokasi memori dinamis (DynArray) untuk model high-poly.
*   **Memory Management:** Custom memory allocators (Arena & Pool) untuk pencegahan kebocoran memori dan optimalisasi performa.
*   **UI System:** Integrasi antarmuka pengguna dasar menggunakan Nuklear IMGUI.

## 4. Requirements

Untuk mengkompilasi dan menjalankan BlankC Engine, Anda membutuhkan:

*   **OS Support:** Windows (teruji penuh), Linux, macOS.
*   **Compiler:** Compiler C yang mendukung standar C11 (GCC, Clang, MSVC).
*   **Graphics API:** Driver kartu grafis yang mendukung **OpenGL 3.3 Core Profile** atau lebih tinggi.
*   **Build System:** CMake (versi 3.16+).
*   **Dependencies (Ter-bundle via FetchContent/Header-only):**
    *   GLFW 3.4 (Window & Input)
    *   GLAD (OpenGL Loader)
    *   stb_image (Texture Loading)
    *   miniaudio (Audio Playback)
    *   Nuklear (Immediate Mode UI)

## 5. Installation & Build Instructions

BlankC Engine menggunakan CMake untuk manajemen build lintas platform.

### A. Clone Repository
```bash
git clone https://github.com/username/BlankC-Engine.git
cd "BlankC Engine"
```

### B. Build Setup (Windows - MSVC)
Buka terminal (Command Prompt / PowerShell) di direktori root engine:

```powershell
# 1. Buat direktori build dan konfigurasi proyek
cmake -B build -S .

# 2. Kompilasi proyek (Mode Debug)
cmake --build build --config Debug
```

*Executable hasil kompilasi (misalnya `fps_game.exe`) akan berada di folder `build\bin\Debug\`.*

### C. Build Setup (Linux / macOS - GCC/Clang)
```bash
# 1. Konfigurasi proyek
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# 2. Kompilasi
cmake --build build
```

## 6. Quick Start (Minimal Engine Loop)

Berikut adalah contoh kerangka kerja (*skeleton*) cara menjalankan BlankC Engine:

```c
#include "engine.h"

int main(void) {
    // 1. Inisialisasi Engine (1280x720 Window)
    Engine* engine = engine_create(1280, 720, "BlankC Engine Demo");
    if (!engine) return 1;

    // 2. Setup (Kamera, Lighting, Background)
    camera_set_position(engine->camera, vec3(0, 1.5f, 5.0f));
    renderer_set_clear_color(engine->renderer, 0.1f, 0.1f, 0.2f, 1.0f);

    // 3. Main Game Loop
    while (engine_running(engine)) {
        // Update input, physics, dan timer
        engine_update(engine);

        // ... Tulis logika game Anda di sini ...

        // Render scene ke layar
        renderer_enable_depth(engine->renderer);
        renderer_draw_scene(engine->renderer, engine->scene, engine->camera);
        
        // Swap buffer layar
        engine_swap(engine);
    }

    // 4. Bersihkan Memori
    engine_destroy(engine);
    return 0;
}
```
