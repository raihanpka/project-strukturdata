# Proyek Akhir Struktur Data  
## Sistem Penjadwalan & Pemesanan Tiket Kereta Api

---

## Daftar Isi

- [Deskripsi](#deskripsi)
- [Fitur Utama](#fitur-utama)
- [Struktur Folder & File](#struktur-folder--file)
- [Cara Kompilasi & Menjalankan](#cara-kompilasi--menjalankan)
- [Alur Penggunaan](#alur-penggunaan)
  - [Sebagai Admin](#sebagai-admin)
  - [Sebagai Pengguna](#sebagai-pengguna)
- [Penjelasan Data & Struktur](#penjelasan-data--struktur)
- [Penjelasan Fitur Data Structure](#penjelasan-fitur-data-structure)
- [Pengembangan & Debugging](pengembangan--debugging)
- [Kredit](#kredit)

---

## Deskripsi

Aplikasi ini adalah sistem penjadwalan dan pemesanan tiket kereta api berbasis terminal/CLI, dikembangkan menggunakan bahasa C++ dan menerapkan berbagai struktur data (stack, queue, vector, dsb).  
Sistem ini memungkinkan admin untuk mengelola jadwal kereta dan tiket penumpang, serta memungkinkan pengguna untuk melihat jadwal, memesan tiket, dan mencari tiket berdasarkan PNR.

---

## Fitur Utama

- **Manajemen Jadwal Kereta**  
  - Tambah, edit, hapus jadwal kereta (oleh admin)
  - Konfirmasi jadwal sebelum aktif (menggunakan stack)
  - Penyimpanan dan pemuatan jadwal dari file CSV

- **Manajemen Tiket Penumpang**  
  - Pemesanan tiket dengan antrian (queue)
  - Proses antrian tiket secara manual/otomatis oleh admin
  - Stack undo untuk input pemesanan tiket (user bisa membatalkan input terakhir)
  - Pencarian tiket berdasarkan PNR
  - Pencarian pesanan berdasarkan data jadwal (asal, tujuan, kereta, tanggal)

- **Validasi Data**  
  - Validasi format tanggal dan waktu
  - Validasi ketersediaan kursi
  - Validasi input menu

- **Penyimpanan Data**  
  - Data jadwal dan tiket disimpan dalam file CSV (`data/jadwal.csv`, `data/pemesanan.csv`)
  - Data dimuat otomatis saat program dijalankan

---

## Struktur Folder & File

```
projectakhir-strukturdata/
│
├── include/                # Header files
│   ├── data_structure/     # Stack, Queue, dll
│   ├── management/         # Jadwal, Tiket, ManagementSystem
│   └── users/              # Admin, User, Role
│
├── src/
│   ├── data_structure/     # Implementasi Stack, Queue
│   ├── management/         # Implementasi Jadwal, Tiket, File IO, ManagementSystem
│   ├── users/              # Implementasi Admin, User, Role
│   ├── main.cpp            # Entry point
│   ├── menu.cpp            # Menu utama
│
├── data/
│   ├── admin_credentials.txt # Data login admin
│   ├── jadwal.csv            # Data jadwal kereta
│   └── pemesanan.csv         # Data tiket penumpang
│
├── Makefile                # Build system
└── README.md               # Dokumentasi ini
```

---

## Cara Kompilasi & Menjalankan

1. **Clone atau Download Repo**
   ```bash
   git clone <repo-url>
   cd projectakhir-strukturdata
   ```

2. **Kompilasi Program**
   ```bash
   make
   ```

3. **Jalankan Program**
   ```bash
   ./SISTEMKAI
   ```

---

## Alur Penggunaan

### Sebagai Admin

1. **Login sebagai Admin**
   - Masukkan username dan password admin (lihat `data/admin_credentials.txt`).

2. **Menu Admin**
   - **Kelola Jadwal Kereta**
     - Tambah jadwal: Isi data lengkap, sistem akan generate kode otomatis.
     - Edit jadwal: Cari berdasarkan kode, ubah data.
     - Hapus jadwal: Cari berdasarkan kode.
     - Semua perubahan harus dikonfirmasi (stack konfirmasi).
   - **Kelola Tiket Penumpang**
     - Proses antrian pemesanan tiket (otomatis/manual).
     - Cek antrian pemesanan.
   - **Lihat Jadwal Kereta**
     - Tampilkan seluruh jadwal atau filter berdasarkan tanggal.

3. **Penyimpanan**
   - Semua perubahan otomatis disimpan ke file setelah aksi.

### Sebagai Pengguna

1. **Menu Pengguna**
   - **Lihat Jadwal Kereta**
     - Tampilkan jadwal berdasarkan tanggal.
   - **Cari Tiket Penumpang**
     - Cari tiket berdasarkan PNR.
   - **Pesan Tiket**
     - Isi data penumpang, asal, tujuan, kereta, tanggal, kursi.
     - Sistem akan mencari jadwal yang cocok dan generate kode.
     - Fitur undo: sebelum konfirmasi, user bisa membatalkan input terakhir.
     - Setelah konfirmasi, pesanan masuk ke antrian dan menunggu diproses admin.

---

## Penjelasan Data & Struktur

### Data Jadwal (Jadwal)
- **Field:** kode, stasiunAsal, stasiunTujuan, namaKereta, tanggal, waktuBerangkat, waktuTiba
- **Disimpan di:** `vector<Jadwal> daftarJadwal`
- **File:** `data/jadwal.csv`

### Data Pemesanan (Pemesanan)
- **Field:** pnr, namaPenumpang, nomorKursi, kodeJadwal, stasiunAsal, stasiunTujuan, namaKereta, tanggal, confirmed
- **Disimpan di:** `vector<Pemesanan> daftarPemesanan`
- **File:** `data/pemesanan.csv`

---

## Penjelasan Fitur Data Structure

- **Stack (Template Manual)**
  - Undo input pemesanan tiket (user bisa membatalkan input sebelum konfirmasi).
  - Stack konfirmasi jadwal (admin harus konfirmasi sebelum jadwal aktif).

- **Queue (Template Manual)**
  - Antrian pemesanan tiket (pesanan masuk ke queue, admin memproses satu per satu).

- **Sorting - Quick Sort (Template Manual)**
  - Mengurutkan jadwal kereta api berdasarkan hari dan jam nya (jadwal masuk ke `vector<Jadwal> daftarJadwal`, lalu diproses dengan fungsi `quicksort`).

- **Hashing dengan Unordered Map & Set (STL)**
  - Melacak data tiket penumpang yang sudah dipesan untuk setiap jadwal.

- **Vector (STL)**
  - Menyimpan daftar jadwal dan daftar pemesanan.


---

## Pengembangan & Debugging

- **Pastikan data jadwal sudah ada sebelum melakukan pemesanan tiket.**
- **Gunakan fitur undo untuk menghindari input salah saat pemesanan.**
- **Jika data tidak muncul saat cek antrian, cek kecocokan kodeJadwal antara pemesanan dan jadwal.**
- **Periksa file CSV jika terjadi error pada load/save data.**
- **Gunakan debug print pada daftarJadwal dan daftarPemesanan jika perlu.**

---

## Kredit

Sistem aplikasi ini dibuat oleh **Kelompok 6** dari **Paralel 1**  
- Raihan Putra Kirana  
- Tristian Yosa  
- Grasela Anggi Asimima Marbun

---