# BlankC Engine Core API Reference

This document summarizes the core functions of each subsystem in the BlankC Engine. For full implementation details, you can refer directly to the respective header (`.h`) files.

---

## 1. Engine Core (`engine.h`)

High-level functions for managing the application lifecycle.

- `Engine* engine_create(int width, int height, const char* title)`
  Creates a window and initializes all subsystems (Renderer, Audio, Physics, etc.).
- `void engine_update(Engine* engine)`
  Processes input, calculates delta time, and updates the Scene and Physics World.
- `void engine_swap(Engine* engine)`
  Swaps the screen buffers after the rendering process is complete.
- `void engine_destroy(Engine* engine)`
  Cleans up all subsystem memory allocated by `engine_create`.
- `int engine_running(Engine* engine)`
  Checks if the window is still active (close button/ESC has not been pressed).

---

## 2. Rendering System

### Renderer (`rendering/renderer.h`)
- `void renderer_clear(Renderer* renderer)`
  Clears the screen using the configured *clear color*.
- `void renderer_set_clear_color(Renderer* renderer, float r, float g, float b, float a)`
- `void renderer_enable_depth(Renderer* renderer)`
  Enables depth testing (Z-buffer) so 3D objects don't incorrectly overlap.
- `void renderer_draw_scene(Renderer* renderer, SceneGraph* graph, Camera* camera)`
  Draws all nodes in the *Scene Graph* using the active *Camera* settings.

### Camera (`rendering/camera.h`)
- `void camera_set_position(Camera* camera, Vec3 pos)`
- `void camera_process_mouse(Camera* camera, float xoffset, float yoffset)`
  Rotates camera orientation based on mouse movement.

---

## 3. Scene System (`scene/scenegraph.h`)

- `SceneGraph* scenegraph_create(void)`
- `SceneNode* scenegraph_add_node(SceneGraph* graph, SceneNode* parent, const char* name)`
  Adds a new entity to the world. If `parent` is NULL, it attaches to the root.
- `void scenegraph_remove_node(SceneGraph* graph, SceneNode* node)`
  Removes a node (and all its descendants) from the scene and frees its memory.
- `SceneNode* scenegraph_find(SceneGraph* graph, const char* name)`
  Searches for a node by name.

---

## 4. Physics System (`physics/physics.h`)

- `RigidBody* physics_world_add_body(PhysicsWorld* world, Vec3 position, Vec3 size, int is_static)`
  Adds a physical collision box. If `is_static = 1`, the object will not be affected by gravity.
- `void physics_world_remove_body(PhysicsWorld* world, RigidBody* body)`
- `void physics_world_update(PhysicsWorld* world, float delta_time)`
  Runs the physics simulation (gravity and AABB collision resolution) for this frame.

---

## 5. Audio System (`audio/audio.h`)

- `AudioSystem* audio_init(void)`
- `void audio_play_sound(AudioSystem* audio, const char* filepath)`
  Plays a sound file (e.g., a gunshot effect). Supports *overlapping*.
- `void audio_play_music(AudioSystem* audio, const char* filepath)`
  Plays a background music file in a *looping* state.
- `void audio_stop_music(AudioSystem* audio)`

---

## 6. Interaction System (`interaction/interaction.h`)

- `SceneNode* interaction_query(InteractionSystem* sys, float max_distance, float* out_distance)`
  Casts an imaginary *ray* from the center of the screen forward up to `max_distance`. Returns a pointer to the first `SceneNode` hit by the ray (if any). Extremely useful for *shooting* mechanics or pressing buttons in a 3D world.

---

## 7. Resource Loading (`resource/loader.h` & `resource/obj_loader.h`)

- `char* asset_path(const char* rel_path)`
  An essential *helper function*. Converts a relative path (e.g., `assets/audio/bgm.mp3`) into an *absolute path* that is guaranteed to be valid from wherever the executable is run. You **must** call `free()` on the returned string.
- `ObjModel* obj_load(const char* path, const char* texture_path)`
  Loads a 3D model and its texture into an object ready to be drawn by the Renderer.