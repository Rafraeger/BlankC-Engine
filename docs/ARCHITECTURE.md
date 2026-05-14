# BlankC Engine Architecture

Dokumen ini menjelaskan bagaimana BlankC Engine diatur, bagaimana *main loop* bekerja, dan bagaimana memori dikelola.

## 1. Project Structure

Engine ini memisahkan antara abstraksi core engine dan contoh implementasi game.

```text
BlankC Engine/
├── assets/         # Resource game (audio, models, shaders, textures)
├── build/          # Folder output kompilasi CMake
├── docs/           # Dokumentasi engine
├── examples/       # Contoh-contoh kecil cara menggunakan fitur spesifik
├── include/        # Header files (.h) - Abstraksi publik engine
│   ├── audio/      
│   ├── core/       # Math, Memory, Log, Time
│   ├── input/      
│   ├── physics/    
│   ├── rendering/  
│   ├── resource/   
│   ├── scene/      
│   └── ui/         
├── libs/           # Dependensi pihak ketiga (GLAD, stb, miniaudio, nuklear)
├── src/            # Implementasi source code (.c) dari engine
└── CMakeLists.txt  # Konfigurasi build system
```

## 2. Main Loop Architecture

BlankC Engine menggunakan pendekatan *programmer-controlled main loop*. Engine tidak menyembunyikan loop; Anda yang menuliskannya di fungsi `main()`.

### Engine Lifecycle

1.  **Inisialisasi (`engine_create`):**
    Mengalokasikan memori untuk seluruh subsistem (Window, Renderer, Scene Graph, Physics World, Audio System, Input, UI).
2.  **Setup Scene:**
    Menambahkan objek ke Scene Graph (`scenegraph_add_node`), memuat model/tekstur, dan mendaftarkan objek ke Physics World.
3.  **Update Phase (`engine_update`):**
    - `time_tick()`: Menghitung delta time.
    - Memproses input (Keyboard/Mouse/Events GLFW).
    - Memperbarui interaksi UI (Nuklear).
    - Memperbarui kamera berdasarkan input.
    - `scenegraph_update()`: Mengalikan matriks transformasi lokal dengan parent untuk mendapatkan *world matrix*.
    - `physics_world_update()`: Menggerakkan *RigidBody* berdasarkan *velocity* & gravitasi, lalu mengecek dan menyelesaikan tabrakan AABB.
4.  **Render Phase (`renderer_draw_scene`):**
    - Membersihkan layar (Color & Depth buffer).
    - Menyiapkan shader uniform (View, Projection, Lights).
    - Melakukan *tree traversal* pada Scene Graph dan memanggil perintah gambar (`glDrawElements`) untuk setiap node yang memiliki Mesh.
    - `engine_swap()`: Menggambar UI di atas layar 3D, lalu menukar buffer (Double Buffering).
5.  **Shutdown (`engine_destroy`):**
    Membersihkan dan mengembalikan semua memori ke OS dengan urutan terbalik dari inisialisasi, diakhiri dengan menghancurkan GL Context.

## 3. Memory Management

BlankC Engine sangat menghindari kebocoran memori (Memory Leaks) dengan beberapa strategi:

1.  **Strict Ownership:**
    - Jika sebuah fungsi memiliki akhiran `_create` atau `_load`, maka *caller* bertanggung jawab untuk memanggil fungsi `_destroy` yang sesuai.
    - Fungsi seperti `asset_path()` mengembalikan string dinamis yang *wajib* di-`free()` setelah digunakan.
2.  **Dynamic Arrays (DynArray):**
    Digunakan secara internal (seperti di `obj_loader.c`) untuk memuat model skala besar tanpa menyebabkan *stack overflow*.
3.  **Custom Allocators (Tersedia via `core/memory.h`):**
    - **Arena Allocator:** Mengalokasikan blok memori besar di awal, lalu membagikannya secara linier. Sangat cepat, namun hanya bisa di-reset secara keseluruhan (tidak bisa melepaskan alokasi individu). Cocok untuk memori yang hidup per-frame.
    - **Pool Allocator:** Mengelola ukuran objek yang seragam (*fixed-size slots*). Sangat baik untuk mencegah fragmentasi memori ketika memunculkan/menghancurkan banyak entitas yang sejenis (misal: partikel atau peluru).

## 4. Coding Style

- **Naming Convention:** Menggunakan `snake_case` untuk variabel dan fungsi. Struct menggunakan `PascalCase`.
- **Modularity:** Setiap subsistem (rendering, audio, physics) dibuat serapat mungkin dengan *state* tersimpan dalam struct *instance* (bukan variabel global).
- **No Hidden Magic:** Kode harus eksplisit. Menghindari makro yang terlalu kompleks jika sebuah fungsi C biasa dapat melakukan hal yang sama.
