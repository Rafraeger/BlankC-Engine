# BlankC Engine Core API Reference

Dokumen ini merangkum fungsi-fungsi utama dari setiap subsistem dalam BlankC Engine. Untuk detail implementasi penuh, Anda dapat merujuk langsung ke file header (`.h`) yang bersangkutan.

---

## 1. Engine Core (`engine.h`)

Fungsi tingkat tinggi untuk mengelola siklus hidup aplikasi.

- `Engine* engine_create(int width, int height, const char* title)`
  Membuat window dan menginisialisasi semua subsistem (Renderer, Audio, Physics, dll).
- `void engine_update(Engine* engine)`
  Memproses input, menghitung delta time, dan memperbarui Scene serta Physics World.
- `void engine_swap(Engine* engine)`
  Menukar (swap) buffer layar setelah proses render selesai.
- `void engine_destroy(Engine* engine)`
  Membersihkan seluruh memori subsistem yang dialokasikan oleh `engine_create`.
- `int engine_running(Engine* engine)`
  Mengecek apakah window masih aktif (belum ditekan tombol close/ESC).

---

## 2. Rendering System

### Renderer (`rendering/renderer.h`)
- `void renderer_clear(Renderer* renderer)`
  Membersihkan layar menggunakan warna *clear color*.
- `void renderer_set_clear_color(Renderer* renderer, float r, float g, float b, float a)`
- `void renderer_enable_depth(Renderer* renderer)`
  Mengaktifkan depth testing (Z-buffer) agar objek 3D tidak tumpang tindih secara salah.
- `void renderer_draw_scene(Renderer* renderer, SceneGraph* graph, Camera* camera)`
  Menggambar seluruh node di *Scene Graph* menggunakan pengaturan *Camera* aktif.

### Camera (`rendering/camera.h`)
- `void camera_set_position(Camera* camera, Vec3 pos)`
- `void camera_process_mouse(Camera* camera, float xoffset, float yoffset)`
  Memutar orientasi kamera berdasarkan gerakan mouse.

---

## 3. Scene System (`scene/scenegraph.h`)

- `SceneGraph* scenegraph_create(void)`
- `SceneNode* scenegraph_add_node(SceneGraph* graph, SceneNode* parent, const char* name)`
  Menambahkan entitas baru ke dunia. Jika `parent` adalah NULL, ia akan dihubungkan ke root.
- `void scenegraph_remove_node(SceneGraph* graph, SceneNode* node)`
  Menghapus node (dan semua turunannya) dari scene dan melepaskan memorinya.
- `SceneNode* scenegraph_find(SceneGraph* graph, const char* name)`
  Mencari node berdasarkan nama.

---

## 4. Physics System (`physics/physics.h`)

- `RigidBody* physics_world_add_body(PhysicsWorld* world, Vec3 position, Vec3 size, int is_static)`
  Menambahkan kotak tabrakan fisik. Jika `is_static = 1`, benda tidak akan terpengaruh gravitasi.
- `void physics_world_remove_body(PhysicsWorld* world, RigidBody* body)`
- `void physics_world_update(PhysicsWorld* world, float delta_time)`
  Menjalankan simulasi fisika (gravitasi dan resolusi tabrakan AABB) untuk frame ini.

---

## 5. Audio System (`audio/audio.h`)

- `AudioSystem* audio_init(void)`
- `void audio_play_sound(AudioSystem* audio, const char* filepath)`
  Memutar file suara (seperti efek tembakan). Mendukung *overlapping*.
- `void audio_play_music(AudioSystem* audio, const char* filepath)`
  Memutar file musik latar secara *looping*.
- `void audio_stop_music(AudioSystem* audio)`

---

## 6. Interaction System (`interaction/interaction.h`)

- `SceneNode* interaction_query(InteractionSystem* sys, float max_distance, float* out_distance)`
  Menembakkan *raycast* (sinar) imajiner dari tengah layar ke depan sejauh `max_distance`. Mengembalikan pointer ke `SceneNode` pertama yang terkena sinar (jika ada). Sangat berguna untuk mekanik *shooting* atau menekan tombol di dunia 3D.

---

## 7. Resource Loading (`resource/loader.h` & `resource/obj_loader.h`)

- `char* asset_path(const char* rel_path)`
  *Helper function* penting. Mengubah path relatif (misal `assets/audio/bgm.mp3`) menjadi *absolute path* yang dijamin valid dari mana saja file executable dijalankan. Anda **harus** memanggil `free()` pada string yang dikembalikan.
- `ObjModel* obj_load(const char* path, const char* texture_path)`
  Memuat model 3D beserta teksturnya menjadi objek yang siap digambar oleh Renderer.
