# Bug Checking & Quality Assurance Plan - BlankC Engine

Dokumen ini berisi rencana sistematis untuk memverifikasi stabilitas engine dan memastikan tidak ada regresi dari perbaikan sebelumnya.

## 1. Verifikasi Regresi (Regression Testing)
Memastikan bug yang sudah diperbaiki di `progress.md` tidak muncul kembali.

- [ ] **Core Math Aliasing:** Tes `mat4_multiply(A, A, B)` dan `mat4_transpose(A, A)`.
- [ ] **Memory Allocation:** Verifikasi `arena_alloc` tidak crash saat mencapai limit dan `NULL` check pada `malloc` berfungsi.
- [ ] **Scene Graph Cleanup:** Pastikan `scenegraph_remove_node` benar-benar menghapus data dan tidak meninggalkan "ghost" object.
- [ ] **Raycasting Accuracy:** Verifikasi tembakan di `fps_game.c` mengenai target secara presisi di berbagai jarak.

## 2. Audit Memori & Stabilitas
Fokus pada kebocoran memori (memory leaks) dan penggunaan resource.

- [ ] **Asset Leak Check:** Load/unload texture 100x dalam satu sesi dan pantau penggunaan RAM.
- [ ] **String Ownership:** Audit penggunaan `strdup` di `scenegraph.c` dan `loader.c`, pastikan setiap `alloc` memiliki `free`.
- [ ] **Long-run Stability:** Jalankan `fps_game.exe` selama 20 menit tanpa restart untuk mengecek akumulasi drift pada `s_fps_timer`.

## 3. Tes Kasus Ekstrim (Edge Case & Stress Test)
Menguji batas kemampuan engine.

- [ ] **Input Spamming:** Tekan input secara acak dan cepat untuk mencari crash pada input handler.
- [ ] **Window Resize:** Ubah ukuran window terus-menerus dan cek apakah rendering tetap stabil.
- [ ] **High-Poly OBJ:** Load model dengan >50.000 vertex untuk mengetes `DynArray` di `obj_loader.c`.
- [ ] **Physics Tunneling:** Gerakkan player/object dengan kecepatan sangat tinggi ke arah dinding.

## 4. Akurasi Visual & Audio
- [ ] **Z-Buffer Test:** Pastikan tidak ada *flickering* pada objek yang bertumpukan.
- [ ] **Normal Map Orientation:** Cek lighting pada semua sisi objek 3D (terutama setelah fix `cube.obj`).
- [ ] **Audio Overlapping:** Mainkan banyak sound effect secara bersamaan untuk mengecek stabilitas `miniaudio`.

## 5. Implementasi Unit Test (Proposisi)
Membuat file test khusus untuk logic non-grafis:
- `tests/test_math.c`
- `tests/test_memory.c`

---
*Target: 0 Critical Bugs, 0 Memory Leaks.*
