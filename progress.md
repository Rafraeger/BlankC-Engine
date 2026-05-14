# Progress — BlankC Engine Bug Audit & Fix

Rangkuman lengkap proses audit dan perbaikan bug pada engine 3D OpenGL berbasis C.

---

## Fase 1: Audit Kode Engine

Dilakukan review menyeluruh terhadap seluruh source code engine (23 file `.c` dan `.h`), mencakup modul:
- Core (math, memory, log, time)
- Rendering (camera, gl_context, mesh, renderer, shader, texture)
- Scene (scenegraph, transform)
- Physics, Input, Resource Loader, OBJ Loader
- UI (Nuklear), Audio (miniaudio), Interaction
- Engine wrapper dan FPS game

**Hasil:** Ditemukan **23 potensi bug** dengan severity Critical (10), Medium (7), dan Low (6).

---

## Fase 2: Perbaikan Bug Engine (13 file diubah)

### Core Math — `src/core/math.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #1 | `mat4_multiply()` aliasing — jika `out == a` atau `out == b`, hasil corrupt | Tambah buffer `Mat4 tmp` internal, `memcpy` ke `out` di akhir |
| #2 | `mat4_transpose()` aliasing — sama seperti di atas | Sama, tambah buffer temp internal |

### Core Memory — `src/core/memory.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #3 | `arena_alloc_aligned()` capacity check salah: `aligned + size - offset > capacity` | Ganti ke `aligned + size > capacity` |
| #4 | `pool_free()` silent leak jika pool penuh | Tambah warning `fprintf(stderr, ...)` |
| #5 | `pool_destroy()` semantik membingungkan | Tambah dokumentasi comment |
| #19 | `arena_create()` / `pool_create()` tidak check `malloc` return | Tambah NULL checks, return NULL jika gagal |

### Core Log — `src/core/log.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #12 | `va_list` digunakan 2x (stdout + file) — UB | Tambah `va_copy()` sebelum konsumsi kedua |

### Core Time — `src/core/time.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #18 | Hanya support Windows (`<windows.h>`) | Tambah fallback `glfwGetTime()` untuk non-Windows |
| #20 | `s_fps_timer = 0` kehilangan sisa waktu (drift) | Ganti ke `s_fps_timer -= 1.0` |

### Rendering Camera — `src/rendering/camera.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #6 | `camera_set_target()` yaw salah: `atan2f(-front.x, -front.z)` | Ganti ke `atan2f(front.z, front.x)` sesuai konvensi `camera_update` |
| #21 | `camera_process_scroll()` arah terbalik (scroll up = zoom out) | Ganti `fov += y_offset` ke `fov -= y_offset` |

### Scene Graph — `src/scene/scenegraph.c` + `include/scene/scenegraph.h`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #9 | `scenegraph_add_node()` simpan pointer `name` tanpa copy — dangling jika sumber stack | Ganti ke `strdup(name)`, tambah `free()` di destroy |
| — | Tidak ada fungsi untuk menghapus node dari scene graph | Tambah `scenegraph_remove_node()` |

### Resource Loader — `src/resource/loader.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #14 | `file_read_text()` tidak check return value `fread` | Check `read_bytes`, gunakan sebagai ukuran actual |
| #22 | `get_exe_dir()` double-strip path jika mixed separator `\` dan `/` | Cari separator terakhir dari kedua jenis, strip sekali |
| #23 | `asset_path()` buffer size dihitung berulang, rawan off-by-one | Hitung sekali ke variabel `full_len` |

### OBJ Loader — `src/resource/obj_loader.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #13 | Stack arrays ~560KB (`ObjVertex[10000]`, dll.) — stack overflow untuk model besar | Ganti ke heap-allocated `DynArray` yang grow otomatis via `realloc` |

### Interaction — `src/interaction/interaction.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #17 | Raycasting pakai `position` lokal, bukan world-space | Ganti ke `transform_get_world_position()` |

### Engine Core — `src/engine.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #8 | `engine_destroy()` — `gl_context_destroy` dipanggil pertama, semua GL cleanup setelahnya jadi UB | Pindahkan `gl_context_destroy` ke **terakhir** |
| #15 | Duplikat handling `KEY_E` di `engine_update` dan `fps_game.c` | Hapus yang di `engine_update`, biarkan game yang handle |

### Camera Controller — `src/camera_controller.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #7 | `asset_path()` leak setiap jump — return value tidak pernah di-`free` | Simpan ke variabel, `free()` setelah pakai |

### FPS Game — `src/fps_game.c`
| Bug | Masalah | Fix |
|-----|---------|-----|
| #7v | `asset_path()` leak di `on_cube_interact` dan `on_shoot` | Sama — simpan dan `free()` |
| #10 | `game_reset()` tidak hapus node enemy lama dari scene graph — ghost rendering | Tambah `scenegraph_remove_node()` sebelum spawn baru |
| #16 | `physics_world_remove_body()` di-call tapi pointer `body` masih disimpan — dangling | Set `g_enemies[i].body = NULL` setelah remove |

---

## Fase 3: Perbaikan Bug Gameplay

### Bug: Model Cube Rusak (bentuk spike/pyramid)
**File:** `assets/models/cube.obj`

**Masalah:**
- Bottom face (y=-0.5) **hilang** — digantikan duplikat left face
- Normal index **tertukar** di beberapa face:
  - Right face pakai normal bottom (0,-1,0)
  - Back face pakai normal right (1,0,0)
  - Duplikat left face pakai normal back (0,0,-1)
- Akibatnya Phong lighting menghitung arah cahaya salah → face terang/gelap terbalik → terlihat seperti spike

**Fix:** Tulis ulang `cube.obj` dengan 6 face yang benar dan normal index yang sesuai orientasi masing-masing face.

---

### Bug: Camera Tidak Bisa Digerakkan
**File:** `src/engine.c`

**Masalah:**
- GLFW input events **tidak pernah di-polling** ke `Input` system
- Nuklear install callback sendiri (`NK_GLFW3_INSTALL_CALLBACKS`), tapi game `Input` struct tidak pernah menerima event keyboard/mouse
- Semua fungsi `input_key_down()`, `input_mouse_dx()`, dll. selalu return 0

**Fix:** Tambah `engine_poll_input()` yang setiap frame:
- Poll keyboard via `glfwGetKey()` (range 32–348)
- Poll mouse position via `glfwGetCursorPos()`, hitung delta manual
- Poll mouse buttons via `glfwGetMouseButton()`
- Feed semua data ke `Input` struct via `input_set_key/mouse/delta/button`

---

### Bug: Camera Tidak Aktif Setelah Start Game
**File:** `src/fps_game.c`

**Masalah:**
- `game_reset()` memanggil `gl_context_lock_mouse(ctx, 1)` tapi **tidak set** `ctrl->mouse_locked = 1`
- Akibatnya `controller_update()` tidak pernah dipanggil (gated oleh `if (ctrl->mouse_locked)`)

**Fix:** Tambah `ctrl->mouse_locked = 1` di `game_reset()`.

---

### Bug: Tembakan Tidak Mengenai Enemy (Raycasting Meleset)
**File:** `src/scene/transform.c`

**Masalah:**
- `transform_get_world_position()` melakukan `mat4_mul_vec4(world_matrix, local_pos)`
- `world_matrix` sudah mengandung translasi (T*R*S), jadi translasi di-apply **2x lipat**
- AABB enemy berada di posisi ~2x dari seharusnya → ray tidak pernah intersect

**Fix:** Ganti ke ekstraksi langsung dari kolom translasi world matrix:
```c
return vec3(t->world_matrix[12], t->world_matrix[13], t->world_matrix[14]);
```

---

### Bug: Dinding & Lantai Naik Saat Ditembak
**File:** `src/fps_game.c`

**Masalah:**
- Di `on_shoot()`, ada kode debug: `hit->transform->position.y += 0.1f`
- Setiap menembak objek non-enemy (dinding, lantai, pilar), posisi Y naik 0.1 unit

**Fix:** Hapus baris debug tersebut.

---

## Fase 4: Quality Assurance & Stability (Audit Final)

### Unit Testing & Regression
Dibuat rangkaian unit test baru untuk memverifikasi stabilitas jangka panjang dan mencegah regresi:
- `tests/test_math.c`: Verifikasi aliasing pada `mat4_multiply` dan `mat4_transpose`.
- `tests/test_memory.c`: Verifikasi limit pada `Arena` dan `Pool` allocator.
- `tests/test_scenegraph.c`: Verifikasi penghapusan node dan subtree tanpa sisa ("ghost objects").
- `tests/test_interaction.c`: Verifikasi akurasi ray-AABB intersection.
- `tests/test_leak.c`: Stress test loading/unloading texture 100x untuk deteksi memory leak.
- `tests/test_audio.c`: Stress test overlapping audio (miniaudio).

### Perbaikan Tambahan (Quality of Life)

| File | Masalah | Fix |
|------|---------|-----|
| `src/rendering/gl_context.c` | Window resize tidak update internal `width`/`height` | Tambah `glfwGetFramebufferSize` di `gl_context_update` |
| `src/resource/loader.c` | Path separator campuran (`\`/`/`) di Windows | Normalisasi semua `/` menjadi `\` pada Windows |
| `src/audio/audio.c` | Debugging audio sulit saat gagal | Tambah error code `ma_result` pada log warning |

### Hasil Audit Final
- **Math & Memory:** 100% Passed. Aliasing dan limit handling sudah solid.
- **Scene Graph:** Node removal terverifikasi bersih.
- **Raycasting:** Akurasi perpotongan Ray-AABB terverifikasi.
- **Memory Leak:** 100 iterasi texture load/unload stabil tanpa kenaikan RAM drastis.
- **Audio:** `miniaudio` stabil untuk overlapping, namun ditemukan `assets/audio/jump.wav` korup (berupa file HTML). `bgm.mp3` normal.
- **Windowing:** Aspect ratio sekarang tetap proporsional setelah resize.

---

## Statistik Akhir

| Kategori | Jumlah |
|----------|--------|
| File yang diubah | 17 |
| Bug/Issue yang diperbaiki | 32 |
| Unit Test baru | 6 |
| **Status Final** | **Stable & Verified** |
