# BlankC Engine Architecture

This document explains how BlankC Engine is organized, how the *main loop* works, and how memory is managed.

## 1. Project Structure

The engine separates core engine abstractions from game implementation examples.

```text
BlankC Engine/
├── assets/         # Game resources (audio, models, shaders, textures)
├── build/          # CMake compilation output folder
├── docs/           # Engine documentation
├── examples/       # Small examples of how to use specific features
├── include/        # Header files (.h) - Public engine abstractions
│   ├── audio/      
│   ├── core/       # Math, Memory, Log, Time
│   ├── input/      
│   ├── physics/    
│   ├── rendering/  
│   ├── resource/   
│   ├── scene/      
│   └── ui/         
├── libs/           # Third-party dependencies (GLAD, stb, miniaudio, nuklear)
├── src/            # Source code implementations (.c) of the engine
└── CMakeLists.txt  # Build system configuration
```

## 2. Main Loop Architecture

BlankC Engine uses a *programmer-controlled main loop* approach. The engine does not hide the loop; you write it explicitly in your `main()` function.

### Engine Lifecycle

1.  **Initialization (`engine_create`):**
    Allocates memory for all subsystems (Window, Renderer, Scene Graph, Physics World, Audio System, Input, UI).
2.  **Scene Setup:**
    Adds objects to the Scene Graph (`scenegraph_add_node`), loads models/textures, and registers objects to the Physics World.
3.  **Update Phase (`engine_update`):**
    - `time_tick()`: Calculates delta time.
    - Processes input (Keyboard/Mouse/GLFW Events).
    - Updates UI interactions (Nuklear).
    - Updates camera based on input.
    - `scenegraph_update()`: Multiplies local transformation matrices with parent matrices to calculate the *world matrix*.
    - `physics_world_update()`: Moves *RigidBodies* based on *velocity* & gravity, then checks and resolves AABB collisions.
4.  **Render Phase (`renderer_draw_scene`):**
    - Clears the screen (Color & Depth buffers).
    - Sets up shader uniforms (View, Projection, Lights).
    - Performs a *tree traversal* on the Scene Graph and calls draw commands (`glDrawElements`) for each node containing a Mesh.
    - `engine_swap()`: Draws the UI on top of the 3D screen, then swaps buffers (Double Buffering).
5.  **Shutdown (`engine_destroy`):**
    Cleans up and returns all memory to the OS in reverse order of initialization, ending with the destruction of the GL Context.

## 3. Memory Management

BlankC Engine strictly avoids Memory Leaks through several strategies:

1.  **Strict Ownership:**
    - If a function ends with `_create` or `_load`, the *caller* is responsible for calling the corresponding `_destroy` or `_free` function.
    - Functions like `asset_path()` return a dynamic string that *must* be `free()`'d after use.
2.  **Dynamic Arrays (DynArray):**
    Used internally (e.g., in `obj_loader.c`) to load large-scale models without causing a *stack overflow*.
3.  **Custom Allocators (Available via `core/memory.h`):**
    - **Arena Allocator:** Allocates a large block of memory upfront, then distributes it linearly. Extremely fast, but can only be reset entirely (cannot free individual allocations). Perfect for per-frame memory lifetimes.
    - **Pool Allocator:** Manages uniform object sizes (*fixed-size slots*). Excellent for preventing memory fragmentation when spawning/destroying many entities of the same type (e.g., particles or projectiles).

## 4. Coding Style

- **Naming Convention:** Uses `snake_case` for variables and functions. Structs use `PascalCase`.
- **Modularity:** Each subsystem (rendering, audio, physics) is encapsulated tightly, with *state* stored in an *instance* struct rather than global variables.
- **No Hidden Magic:** Code must be explicit. Avoids overly complex macros if a standard C function can achieve the same result.