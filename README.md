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
2. Biên dịch và chạy (trong Visual Studio Developer Command Prompt/PowerShell):
   ```bash
   cmake -B build -S .
   cmake --build build
   ```
### Linux
1. Cài đặt GCC và CMake (nếu chưa có):
   ```bash
   # Ubuntu/Debian
   sudo apt-get install build-essential cmake
   ```
2. Biên dịch và chạy:
   ```bash
   cmake -B build -S .
   cmake --build build
   ```
### macOS
1. Cài đặt Xcode Command Line Tools:
   ```bash
   xcode-select --install
   ```
2. Biên dịch và chạy:
   ```bash
   cmake -B build -S .
   cmake --build build
   ```
## Chạy test

Để chạy tất cả test:
```bash
ctest --test-dir build --output-on-failure
```

Để chạy test cho một chương trình cụ thể (vd: bài maze tuần 1):
```bash
ctest --test-dir build -R week1_maze --output-on-failure
```
