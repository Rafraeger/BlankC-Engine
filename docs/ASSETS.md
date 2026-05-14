# Asset Management & Resource Loading

Dokumen ini menjelaskan bagaimana BlankC Engine memuat dan mengelola aset dari disk ke dalam memori komputer (RAM) dan memori grafis (VRAM). Semua fungsi pemuatan aset berada di bawah modul `resource/` (`loader.c` dan `obj_loader.c`).

## 1. Path Resolution (`asset_path`)

Karena program C dapat dijalankan dari berbagai direktori kerja (*working directory*), mengandalkan *relative path* murni (seperti `"assets/texture.png"`) sering menyebabkan *File Not Found*.

Untuk mengatasi ini, BlankC menggunakan fungsi **`asset_path(const char* rel_path)`**.
- Fungsi ini secara dinamis mencari letak file *executable* Anda.
- Ia akan berjalan mundur ke atas struktur folder (maksimal 20 tingkat) mencari folder bernama `assets/`.
- Setelah ditemukan, ia menggabungkan path tersebut untuk menghasilkan **Absolute Path**.
- Pada Windows, fungsi ini secara otomatis menormalkan pemisah path (mengubah `/` menjadi `\`) untuk mencegah masalah kompatibilitas dengan library pihak ketiga.

**Aturan Emas:** Setiap string yang dikembalikan oleh `asset_path()` dialokasikan secara dinamis (`malloc`). Anda **wajib** memanggil `free()` setelah selesai menggunakannya untuk mencegah kebocoran memori.

## 2. Memuat Tekstur (Textures)

BlankC menggunakan **`stb_image`** untuk mengurai file gambar (PNG, JPG, BMP) menjadi array pixel.

1.  **`texture_load()`:** Membaca file gambar ke RAM CPU (`TextureData`).
2.  **`texture_create_file()`:** Ini adalah *wrapper* utama. Fungsi ini memanggil `texture_load`, lalu mengambil data piksel tersebut dan mengirimkannya ke VRAM GPU melalui `glTexImage2D`. Fungsi ini kemudian memanggil `texture_data_free()` untuk membuang salinan RAM, menyisakan hanya referensi GPU (berupa `GLuint id` di dalam struct `Texture`).
3.  **Mipmaps:** Engine secara otomatis menghasilkan mipmaps (`glGenerateMipmap`) untuk setiap tekstur yang dimuat, sehingga tekstur terlihat halus saat dilihat dari jauh.
4.  **Format:** Mendukung tekstur RGB (3 channel) dan RGBA (4 channel, mendukung transparansi).

## 3. Memuat Model 3D (OBJ Files)

BlankC memiliki parser OBJ internal (`obj_loader.c`) yang dirancang khusus untuk membaca file Wavefront `.obj`.

### Fitur Parser OBJ:
- Mendukung ekstraksi Vertices (`v`), Texture Coordinates (`vt`), Normals (`vn`), dan Faces (`f`).
- Saat ini **hanya** mendukung wajah berbentuk **Segitiga (Triangles)**. Pastikan untuk men-centang opsi "Triangulate Faces" saat mengekspor model Anda dari Blender atau software 3D lainnya.

### Manajemen Memori Pemuatan:
Membaca model besar berpotensi menyebabkan *Stack Overflow* jika menggunakan array statis. Oleh karena itu, parser `obj_load` menggunakan struktur **`DynArray`** (Dynamic Array).
- `DynArray` memulai alokasi di heap dengan ukuran kecil.
- Jika kapasitas penuh saat membaca baris baru dari file `.obj`, ia otomatis melipatgandakan ukurannya menggunakan `realloc()`.
- Setelah file selesai dibaca, data dari berbagai `DynArray` dirakit menjadi format `Vertex` tunggal yang siap dikirim ke GPU (`mesh_create`).
- Semua `DynArray` sementara (*scratch memory*) langsung di-`free()`.

## 4. Keterbatasan (Limitations)

- **Tidak Ada Resource Cache/Manager:** Jika Anda memuat `"cube.obj"` 10 kali menggunakan `obj_load()`, engine akan membaca file tersebut dari disk 10 kali dan menduplikasi data di VRAM sebanyak 10 kali. Sangat disarankan Anda memuat model/tekstur satu kali ke dalam variabel, lalu memberikan pointer (referensi) yang sama ke berbagai `SceneNode`.
- **Material MTL:** Parser OBJ saat ini tidak membaca file material (`.mtl`). Pengaturan tekstur harus dilakukan secara manual di kode C dengan menetapkan struct `Texture*` ke `SceneNode->texture`.
