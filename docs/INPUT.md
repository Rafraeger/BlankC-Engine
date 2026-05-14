# Input System

BlankC Engine menggunakan modul `Input` (`input.c` dan `input.h`) untuk menangani semua interaksi dari pengguna (keyboard dan mouse). Modul ini bertindak sebagai lapisan abstraksi di atas event system GLFW.

## 1. Arsitektur Polling

Berbeda dengan sistem berbasis event (*event-driven*) murni di mana Anda mendaftarkan fungsi *callback*, BlankC Engine menggunakan sistem **Polling**.

Pada setiap iterasi *main loop*, fungsi `engine_poll_input(Engine* engine)` (di dalam `engine.c`) akan berjalan dan melakukan sinkronisasi state dari GLFW ke dalam struct `Input` internal kita.

Alur kerjanya:
1.  **Baca dari OS:** GLFW membaca status perangkat keras (tombol mana yang ditekan, posisi mouse).
2.  **Simpan State:** Nilai-nilai ini disimpan ke dalam array boolean (untuk keyboard) dan float (untuk mouse) di dalam `Input` struct.
3.  **Gunakan di Game:** Kode game Anda (misalnya di `camera_controller.c` atau `fps_game.c`) cukup mengecek array state tersebut, tanpa perlu memanggil fungsi GLFW secara langsung.

## 2. Keyboard Input

Pengecekan keyboard dilakukan dengan menanyakan status tombol (Key Code) tertentu. BlankC Engine menggunakan definisi Key Code yang sama persis dengan GLFW (misal: `KEY_W`, `KEY_SPACE`, `KEY_ESCAPE`).

Fungsi utama:
- `int input_key_down(Input* input, int key_code)`: Mengembalikan nilai benar (1) jika tombol sedang ditahan pada frame ini.
- `int input_key_pressed(Input* input, int key_code)`: Mengembalikan nilai benar (1) **hanya pada frame pertama** saat tombol ditekan. Sangat berguna untuk aksi "sekali tekan" seperti menembak atau melompat.

## 3. Mouse Input

Sistem input melacak dua jenis data mouse:
1.  **Tombol Mouse:** Dikontrol via `input_mouse_down()` dan `input_mouse_pressed()` mirip seperti keyboard. Tombol 0 biasanya Kiri, tombol 1 Kanan.
2.  **Pergerakan (Delta):**
    - `input_mouse_dx()` dan `input_mouse_dy()` mengembalikan jarak pergerakan kursor mouse relatif terhadap frame sebelumnya.
    - Nilai ini sangat krusial untuk implementasi kamera *First Person Shooter* (FPS) atau *free-look*.

## 4. Mouse Locking (Kursor Disembunyikan)

Untuk game 3D eksplorasi, Anda biasanya tidak ingin kursor mouse terlihat dan keluar dari jendela game saat memutar kamera.

Anda dapat mengontrol visibilitas kursor melalui modul GL Context:
- `gl_context_lock_mouse(engine->context, 1)`: Menyembunyikan kursor dan menguncinya di tengah layar. Pergerakan mouse akan terus dihitung dan dikembalikan oleh `input_mouse_dx/dy` tanpa batas layar.
- `gl_context_lock_mouse(engine->context, 0)`: Memunculkan kembali kursor dan membiarkannya bergerak bebas (misalnya saat membuka Main Menu).
