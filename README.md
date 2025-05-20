# Bài tập Thuật toán Ứng dụng
Bài tập trên trang hustack.soict.ai của lớp IT3170 - Thuật toán ứng dụng

## Cấu trúc thư mục

- `src/`: Thư mục chứa mã nguồn
  - `week*/`: Bài tập theo tuần
  - `testing/`: Framework test
- `tests/`: Thư mục chứa test cases
  - `week*/`: Test cases cho từng tuần
- `build/`: Thư mục chứa file biên dịch

## Chạy trên máy local
### Windows
1. Cài đặt [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/?q=build+tools)
2. Cài đặt Make (Tớ thích dùng [Scoop](https://github.com/ScoopInstaller/Scoop): `scoop install make`)
3. Biên dịch và chạy (trong Visual Studio Developer Command Prompt/PowerShell):
   ```bash
   make clean
   make all
   ```
### Linux
1. Cài đặt GCC và Make (nếu chưa có):
   ```bash
   # Ubuntu/Debian
   sudo apt-get install build-essential make
   ```
2. Biên dịch và chạy:
   ```bash
   make clean
   make all
   ```
### macOS
1. Cài đặt Xcode Command Line Tools:
   ```bash
   xcode-select --install
   ```
2. Biên dịch và chạy:
   ```bash
   make clean
   make all
   ```
## Chạy test

Để chạy test cho một chương trình cụ thể:
```bash
make test week1 maze
```

Để chạy tất cả test:
```bash
make test-all
```