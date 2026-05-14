# Rendering Pipeline & System

Dokumen ini menjelaskan bagaimana sistem grafis BlankC Engine bekerja dari memori hingga tampil ke layar.

## 1. Konsep Dasar Rendering

BlankC Engine menggunakan sistem **Forward Rendering** murni dengan OpenGL 3.3 Core Profile. Tidak ada deferred rendering atau post-processing rumit secara default, menjaga pipeline tetap sangat ringan dan mudah dimengerti.

Komponen utama rendering terdiri dari:
1.  **Renderer (`renderer.h`):** Manager utama yang memegang state OpenGL (seperti *clear color*, status depth test, dan shader yang sedang aktif).
2.  **Shader (`shader.h`):** Pembungkus (wrapper) untuk program shader GLSL (Vertex & Fragment).
3.  **Mesh (`mesh.h`):** Representasi geometri 3D. Mengelola Vertex Array Object (VAO), Vertex Buffer Object (VBO), dan Element Buffer Object (EBO).
4.  **Texture (`texture.h`):** Pembungkus untuk gambar 2D yang diload ke VRAM.

## 2. Rendering Pipeline

Proses menggambar satu frame (`renderer_draw_scene`) melewati tahapan berikut:

1.  **Clear Screen:** 
    `renderer_clear()` dipanggil untuk membersihkan color buffer dan depth buffer menggunakan warna latar belakang yang telah ditentukan.
2.  **Bind Camera & Shader:**
    Kamera menghitung matriks *View* dan *Projection*. Matriks ini kemudian dikirim ke GPU melalui uniform pada shader yang sedang aktif (`uView` dan `uProjection`).
3.  **Scene Graph Traversal:**
    Renderer menelusuri setiap `SceneNode` di dalam `SceneGraph`.
4.  **Draw Call per Node:**
    Jika sebuah `SceneNode` memiliki data `Mesh` (tersimpan di `node->user_data`):
    - Matriks *World* (transformasi absolut objek) dikirim ke uniform `uModel`.
    - Jika node memiliki `Texture`, tekstur tersebut di-bind (slot 0) dan uniform `uUseTexture` diset ke 1. Jika tidak, warna fallback (`uColor`) digunakan.
    - Fungsi `mesh_render()` dipanggil, yang akan melakukan bind pada VAO mesh tersebut dan mengeksekusi perintah `glDrawElements`.
5.  **Swap Buffers:**
    Setelah seluruh node digambar, `engine_swap()` dipanggil untuk menampilkan *back buffer* ke layar.

## 3. Shader System

BlankC Engine menggunakan sistem shader monolitik (satu shader besar untuk banyak hal) pada implementasi dasarnya (`assets/shaders/basic.vert` dan `basic.frag`).

### Vertex Attributes
Setiap vertex yang dikirim ke shader memiliki layout (sesuai definisi di `mesh.c`):
- `layout(location = 0)`: Posisi vertex (`vec3`)
- `layout(location = 1)`: Koordinat UV/Tekstur (`vec2`)
- `layout(location = 2)`: Normal (`vec3`)

### Uniforms Wajib
Shader standar BlankC Engine mengharapkan uniform berikut:
- **Matriks Transformasi:** `uModel`, `uView`, `uProjection`
- **Lighting & Material:** `uLightPos`, `uLightColor`, `uViewPos`
- **Tekstur:** `uTexture` (sampler2D), `uUseTexture` (int flag), `uColor` (vec3 fallback)

## 4. Camera System

Kamera dalam BlankC Engine (`camera.c`) pada dasarnya adalah sistem yang mengelola posisi 3D (`vec3`) dan sudut rotasi (Yaw & Pitch).

- Kamera menghitung **Front Vector**, **Right Vector**, dan **Up Vector** berdasarkan Yaw dan Pitch setiap frame.
- **View Matrix** dihasilkan menggunakan fungsi matematika `mat4_look_at()`.
- **Projection Matrix** dihasilkan menggunakan `mat4_perspective()`, dengan parameter FOV (Field of View) dan aspek rasio layar.

*Catatan: Secara internal, sumbu kamera selalu menganggap Y sebagai UP, dan -Z sebagai FORWARD (Right-handed coordinate system standar OpenGL).*
