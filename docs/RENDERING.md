# Rendering Pipeline & System

This document explains how the BlankC Engine graphics system works from memory to the screen.

## 1. Basic Rendering Concepts

BlankC Engine uses a pure **Forward Rendering** system with OpenGL 3.3 Core Profile. There is no deferred rendering or complex post-processing by default, keeping the pipeline extremely lightweight and easy to understand.

The main rendering components consist of:
1.  **Renderer (`renderer.h`):** The main manager that holds the OpenGL state (such as *clear color*, depth test status, and the currently active shader).
2.  **Shader (`shader.h`):** A wrapper for GLSL shader programs (Vertex & Fragment).
3.  **Mesh (`mesh.h`):** Represents 3D geometry. Manages the Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO).
4.  **Texture (`texture.h`):** A wrapper for 2D images loaded into VRAM.

## 2. Rendering Pipeline

The process of drawing a single frame (`renderer_draw_scene`) goes through the following stages:

1.  **Clear Screen:** 
    `renderer_clear()` is called to clear the color buffer and depth buffer using the predefined background color.
2.  **Bind Camera & Shader:**
    The camera calculates the *View* and *Projection* matrices. These matrices are then sent to the GPU via uniforms on the active shader (`uView` and `uProjection`).
3.  **Scene Graph Traversal:**
    The renderer iterates through every `SceneNode` inside the `SceneGraph`.
4.  **Draw Call per Node:**
    If a `SceneNode` has `Mesh` data (stored in `node->user_data`):
    - The *World* matrix (absolute object transformation) is sent to the `uModel` uniform.
    - If the node has a `Texture`, the texture is bound (slot 0) and the `uUseTexture` uniform is set to 1. Otherwise, a fallback color (`uColor`) is used.
    - The `mesh_render()` function is called, which binds the mesh's VAO and executes the `glDrawElements` command.
5.  **Swap Buffers:**
    After all nodes are drawn, `engine_swap()` is called to present the *back buffer* to the screen.

## 3. Shader System

BlankC Engine uses a monolithic shader system (one large shader for many things) in its basic implementation (`assets/shaders/basic.vert` and `basic.frag`).

### Vertex Attributes
Every vertex sent to the shader has a layout (as defined in `mesh.c`):
- `layout(location = 0)`: Vertex Position (`vec3`)
- `layout(location = 1)`: UV/Texture Coordinates (`vec2`)
- `layout(location = 2)`: Normal (`vec3`)

### Required Uniforms
The standard BlankC Engine shader expects the following uniforms:
- **Transformation Matrices:** `uModel`, `uView`, `uProjection`
- **Lighting & Material:** `uLightPos`, `uLightColor`, `uViewPos`
- **Textures:** `uTexture` (sampler2D), `uUseTexture` (int flag), `uColor` (vec3 fallback)

## 4. Camera System

The camera in BlankC Engine (`camera.c`) is basically a system that manages a 3D position (`vec3`) and rotation angles (Yaw & Pitch).

- The camera calculates the **Front Vector**, **Right Vector**, and **Up Vector** based on Yaw and Pitch every frame.
- The **View Matrix** is generated using the `mat4_look_at()` math function.
- The **Projection Matrix** is generated using `mat4_perspective()`, with FOV (Field of View) and screen aspect ratio as parameters.

*Note: Internally, the camera axes always assume Y is UP, and -Z is FORWARD (Standard OpenGL right-handed coordinate system).*