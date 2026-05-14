# BlankC Engine

## 1. Introduction

**BlankC Engine** is a 3D game framework written entirely in C. This engine is designed from the ground up to provide a strong foundation for developers who want to build 3D games or interactive applications without the overhead of massive game engines.

The main goal of this engine is to provide a solid framework for rendering, interaction, basic physics, and audio, while still giving the programmer full control over the *main loop* and memory management.

## 2. Engine Philosophy

BlankC Engine is built on several core philosophies:

*   **Code-First:** No visual editor (GUI). Game logic, object placement, and scene setup are written purely in C code.
*   **Lightweight & Minimal Abstraction:** This engine is extremely lightweight. Abstraction layers are kept as thin as possible so developers understand exactly what is happening behind the scenes (no "magic").
*   **Low-Level Control:** Programmers have full control over the application lifecycle (*main loop*), memory allocation (via custom Arena/Pool allocators), and the execution order of systems (physics, update, render).
*   **Programmer-Oriented:** Designed specifically for programmers who love full control and want to learn how a game engine works at a fundamental level.

## 3. Features

Despite being lightweight, BlankC Engine comes equipped with essential features:

*   **Renderer:** Forward rendering pipeline using OpenGL 3.3 Core Profile. Supports custom shaders, texturing, and depth testing.
*   **Audio System:** Stable audio playback for sound effects (WAV) and background music (MP3) with overlapping support, powered by `miniaudio`.
*   **Physics & Collision:** Basic physics system with Dynamic RigidBodies, AABB (Axis-Aligned Bounding Box) collision detection, and Minimum Translation Vector (MTV) resolution.
*   **Interaction & Raycasting:** High-precision raycasting from the camera for interacting with objects in the 3D world.
*   **Scene System:** Hierarchy-based scene management (Scene Graph) with Transform system (Position, Rotation, Scale) and parent-child inheritance.
*   **Asset Loader:** Efficient loading of 3D models (`.obj`) and textures. Supports dynamic memory allocation (DynArray) for high-poly models.
*   **Memory Management:** Custom memory allocators (Arena & Pool) to prevent memory leaks and optimize performance.
*   **UI System:** Basic user interface integration using Nuklear IMGUI.

## 4. Requirements

To compile and run BlankC Engine, you need:

*   **OS Support:** Windows (fully tested), Linux, macOS.
*   **Compiler:** A C compiler that supports the C11 standard (GCC, Clang, MSVC).
*   **Graphics API:** Graphics card drivers that support **OpenGL 3.3 Core Profile** or higher.
*   **Build System:** CMake (version 3.16+).
*   **Dependencies (Bundled via FetchContent/Header-only):**
    *   GLFW 3.4 (Window & Input)
    *   GLAD (OpenGL Loader)
    *   stb_image (Texture Loading)
    *   miniaudio (Audio Playback)
    *   Nuklear (Immediate Mode UI)

## 5. Installation & Build Instructions

BlankC Engine uses CMake for cross-platform build management.

### A. Clone Repository
```bash
git clone https://github.com/Rafraeger/BlankC-Engine.git
cd "BlankC Engine"
```

### B. Build Setup (Windows - MSVC)
Open a terminal (Command Prompt / PowerShell) in the engine's root directory:

```powershell
# 1. Create a build directory and configure the project
cmake -B build -S .

# 2. Compile the project (Debug Mode)
cmake --build build --config Debug
```

*The compiled executable (e.g., `fps_game.exe`) will be located in the `build\bin\Debug\` folder.*

### C. Build Setup (Linux / macOS - GCC/Clang)
```bash
# 1. Configure the project
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# 2. Compile
cmake --build build
```

## 6. Quick Start (Minimal Engine Loop)

Here is a skeleton example of how to run BlankC Engine:

```c
#include "engine.h"

int main(void) {
    // 1. Initialize Engine (1280x720 Window)
    Engine* engine = engine_create(1280, 720, "BlankC Engine Demo");
    if (!engine) return 1;

    // 2. Setup (Camera, Lighting, Background)
    camera_set_position(engine->camera, vec3(0, 1.5f, 5.0f));
    renderer_set_clear_color(engine->renderer, 0.1f, 0.1f, 0.2f, 1.0f);

    // 3. Main Game Loop
    while (engine_running(engine)) {
        // Update input, physics, and timer
        engine_update(engine);

        // ... Write your game logic here ...

        // Render the scene to the screen
        renderer_enable_depth(engine->renderer);
        renderer_draw_scene(engine->renderer, engine->scene, engine->camera);
        
        // Swap screen buffers
        engine_swap(engine);
    }

    // 4. Cleanup Memory
    engine_destroy(engine);
    return 0;
}
```