# Physics & Collision System

Dokumen ini menguraikan arsitektur sistem fisika dasar yang digunakan dalam BlankC Engine. Sistem ini sengaja dibuat sederhana untuk efisiensi dan kemudahan modifikasi, berfokus pada dinamika gerakan dan deteksi tumbukan kotak (Bounding Box).

## 1. Arsitektur Fisika

Sistem fisika BlankC Engine bukanlah physics engine penuh seperti PhysX atau Bullet. Ini adalah sistem fisika *custom* yang ditangani dalam modul `physics.c`.

Komponen utama:
1.  **PhysicsWorld:** Manager utama yang menyimpan daftar semua entitas fisik yang aktif di dunia, serta parameter global seperti vektor gravitasi.
2.  **RigidBody:** Komponen yang melekat pada objek yang memberikan sifat fisik. Menyimpan posisi, kecepatan (*velocity*), ukuran ruang (untuk tabrakan), dan status (*static* atau *dynamic*).

## 2. Update Loop Fisika

Pembaruan fisika terjadi di dalam fungsi `physics_world_update()`, yang dipanggil satu kali setiap *frame* (dikendalikan oleh delta time).

Proses pembaruan bekerja dalam tiga tahap utama untuk setiap *RigidBody* yang dinamis (`is_static == 0`):

1.  **Integrasi Gaya (Gravity):**
    Kecepatan (*velocity*) dari *RigidBody* ditambahkan dengan percepatan gravitasi yang dikalikan dengan delta time.
    `Velocity = Velocity + (Gravity * DeltaTime)`
2.  **Integrasi Posisi:**
    Posisi *RigidBody* diperbarui berdasarkan kecepatan saat ini dikalikan dengan delta time.
    `Position = Position + (Velocity * DeltaTime)`
3.  **Resolusi Tumbukan (Collision Resolution):**
    Setelah posisi baru dihitung, sistem akan mengecek apakah posisi baru ini menyebabkan *RigidBody* tumpang tindih dengan *RigidBody* lain.

## 3. Deteksi Tabrakan AABB

BlankC Engine menggunakan bentuk tabrakan **AABB (Axis-Aligned Bounding Box)**. AABB adalah kotak 3D murni yang sisi-sisinya selalu sejajar dengan sumbu X, Y, dan Z (tidak bisa miring/berotasi). Ini membuatnya sangat cepat untuk dihitung.

### Mekanisme Tumbukan

Jika dua AABB tumpang tindih (`test_aabb_overlap`):
1. Sistem menghitung **MTV (Minimum Translation Vector)**. MTV adalah vektor terkecil yang diperlukan untuk memisahkan kedua kotak yang saling bertabrakan.
2. Jika benda bergerak menabrak benda **statis** (dinding, lantai): 
   - Benda bergerak didorong keluar sebesar MTV.
   - Kecepatan pada sumbu tabrakan di-nol-kan untuk menghentikan gerakan paksa (mencegah benda terus mencoba menembus).
3. Jika benda bergerak menabrak benda **dinamis** lain:
   - Kedua benda didorong saling menjauh, masing-masing menanggung setengah dari nilai MTV.

## 4. Keterbatasan (Limitations)

Karena sifatnya yang sederhana, sistem ini memiliki beberapa keterbatasan penting yang harus dipahami oleh programmer:

- **Tunneling:** Objek yang bergerak dengan kecepatan sangat tinggi dapat menembus dinding tipis dalam satu frame (karena pengecekan hanya terjadi pada posisi awal dan akhir frame, bukan *Continuous Collision Detection*). Solusi: gunakan dinding yang tebal atau batasi batas kecepatan maksimum objek.
- **Tidak Ada Rotasi Fisik:** Kotak AABB tidak merespons rotasi. Jika Anda memutar model secara visual di scene, kotak tabrakannya tetap tidak berubah. Sistem ini belum mendukung OBB (Oriented Bounding Box) atau tabrakan berbasis *mesh*.
- **Tidak Ada Friksi/Pantulan:** Sistem saat ini tidak mensimulasikan gaya gesek (friksi) permukaan atau sifat memantul (*bounciness*). Semua benda akan meluncur atau berhenti sepenuhnya berdasarkan modifikasi kecepatan manual.
