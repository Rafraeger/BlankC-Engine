# Rencana Pengembangan Engine 3D (6 Fase)

Berikut rencana pengembangan yang dibagi menjadi 6 fase:

---

## Phase 1: Project Setup & Infrastructure

- CMakeLists.txt dengan cross-platform support
- Struktur direktori & file skeleton
- Setup dependencies (GLFW, GLAD, stb_image)
- Build system test: compile & run kosong
- Error logging utility

---

## Phase 2: Window, Context & Input

- GLFW window creation (resize, close, VSync)
- GLAD OpenGL 4.5 context loader
- Input system: keyboard polling, mouse pos/buttons
- Main loop scaffold (run, update, swap, destroy)

**Deliverable:** Window muncul, bisa ditutup dengan ESC

---

## Phase 3: Math Core & Shaders

- math.h: Vec2/Vec3/Vec4, Mat4 ops (mul, translate, rotate, scale)
- math.h: perspective(), lookAt(), normalize()
- Shader system: file load, compile, link, uniform setters
- Basic vertex + fragment shader (color only)

**Deliverable:** Bisa compile shader dan set uniforms

---

## Phase 4: Rendering Pipeline (Hello Triangle)

- Mesh system: VAO/VBO/EBO creation & cleanup
- Camera system: position, rotation, view/projection matrices
- Renderer: draw calls with shader + mesh + camera
- examples/triangle.c: render colored triangle
- examples/quad.c: render textured quad

**Deliverable:** Triangle & textured quad tampil di layar

---

## Phase 5: Scene Graph & Resource Loading

- Transform node: local/world position, rotation, scale
- Scene graph: parent-child hierarchy, traversable tree
- OBJ model loader (vertices, normals, texcoords)
- Texture loader via stb_image
- Renderer extended: render scene graph with transforms

**Deliverable:** Load & render OBJ model dengan texture

---

## Phase 6: Polish & Example Demo

- Engine wrapper API (engine_create, engine_update, dll)
- FPS camera controller (WASD + mouse look)
- examples/demo.c: room scene dengan textured cubes
- Timer/delta time, FPS counter display
- Memory cleanup, leak prevention

**Deliverable:** Playable 3D demo scene

---

## Dependencies per Fase

| Fase | Dependencies |
|------|-------------|
| 1 | CMake only |
| 2 | GLFW3, GLAD |
| 3 | none |
| 4 | GLFW, GLAD |
| 5 | stb_image |
| 6 | semua |

---

## Estimasi Kompleksitas

- **Fase 1-2:** Setup - fondasi, harus solid dulu
- **Fase 3-4:** Rendering - inti engine
- **Fase 5-6:** Fitur - scene graph + polish

---

