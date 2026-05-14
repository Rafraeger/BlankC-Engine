# UI System (Nuklear)

BlankC Engine mengintegrasikan **Nuklear** (sebuah *immediate mode GUI library*) untuk menangani antarmuka pengguna seperti Main Menu, HUD, dan panel Debugging.

## 1. Immediate Mode GUI (IMGUI)

Tidak seperti sistem UI tradisional (Retained Mode) di mana Anda membuat "objek tombol" di awal lalu meng-update text-nya, **Immediate Mode GUI** berarti Anda mendefinisikan dan menggambar UI secara terus-menerus setiap *frame* di dalam *main loop*.

Keuntungan IMGUI:
- Tidak ada state UI yang perlu dikelola secara terpisah.
- Nilai variabel langsung diikat (bind) ke elemen UI (misalnya slider langsung mengubah variabel `float light_intensity`).
- Sangat cepat untuk membuat alat *debugging* atau antarmuka *in-game*.

## 2. Inisialisasi

Nuklear diinisialisasi secara otomatis oleh `engine_create()`. Objek utama Nuklear (`struct nk_context*`) dapat diakses melalui `engine->ui->ctx`.

## 3. Alur Penggambaran UI (UI Flow)

Siklus pembuatan UI per frame dalam BlankC Engine:

1.  **New Frame:** `ui_new_frame()` dipanggil di awal `engine_update()`. Ini mengumpulkan semua input mouse/keyboard dan menyiapkannya untuk Nuklear.
2.  **Deklarasi Jendela (Window):** Anda memulai jendela baru menggunakan `nk_begin()`.
3.  **Deklarasi Layout & Elemen:** Anda menentukan layout (berapa kolom per baris) menggunakan `nk_layout_row_dynamic()`, lalu menambahkan elemen (Teks, Tombol, Slider).
4.  **Tutup Jendela:** Anda mengakhiri deklarasi jendela dengan `nk_end()`.
5.  **Render:** Pada akhir frame, `engine_swap()` akan memanggil `ui_render()`, yang mengambil semua perintah deklarasi dari langkah 2-4 dan mengirimkannya ke GPU (OpenGL) untuk digambar di atas scene 3D.

## 4. Contoh Penggunaan

Berikut adalah contoh pembuatan Main Menu sederhana:

```c
struct nk_context* ctx = engine->ui->ctx;

// Mulai deklarasi UI
if (nk_begin(ctx, "Main Menu", nk_rect(50, 50, 200, 150),
    NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE)) 
{
    // Set 1 kolom per baris, tinggi 40 pixel
    nk_layout_row_dynamic(ctx, 40, 1);
    
    // Elemen Teks
    nk_label(ctx, "Welcome to BlankC", NK_TEXT_CENTERED);
    
    // Elemen Tombol
    if (nk_button_label(ctx, "START GAME")) {
        // Logika ketika tombol Start ditekan
        g_state = STATE_PLAYING;
    }
}
nk_end(ctx); // Wajib dipanggil untuk menutup nk_begin
```

## 5. Menangani Konflik Input (Mouse Capture)

Saat Anda memiliki UI (seperti jendela Debug) yang menumpuk di atas game 3D, Anda tidak ingin aksi mengklik tombol di UI juga dianggap sebagai "menembak senjata" di dalam game.

Gunakan fungsi ini untuk mengecek apakah mouse sedang berinteraksi dengan UI:
```c
if (!ui_want_capture_mouse(engine->ui)) {
    // Mouse tidak berada di atas UI Nuklear, aman untuk input game 3D
    if (input_mouse_pressed(engine->input, 0)) shoot_gun();
}
```
