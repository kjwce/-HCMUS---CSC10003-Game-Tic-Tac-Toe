# -HCMUS---CSC10003-Game-Tic-Tac-Toe
# 🎮 Trò Chơi Caro (Tic Tac Toe) - C++ & SFML

Đồ án nhóm triển khai trò chơi Caro với nhiều chế độ chơi:  
- **Cổ điển (Classic)**  
- **Bom (Bomb Mode)**  
- **PvP (Người chơi vs Người chơi)**  
- **PvE (Người chơi vs Máy)**  

Dự án sử dụng thư viện đồ họa và âm thanh **SFML**.

---

## 📦 Yêu cầu hệ thống

Để biên dịch và chạy dự án này, bạn cần có:

- **Trình biên dịch C++:** GCC (khuyến nghị MinGW trên Windows) hỗ trợ chuẩn C++17.  
- **Thư viện SFML:** Phiên bản 2.6.2 (hoặc tương đương) phù hợp với compiler.  
  - Ví dụ: với GCC 13.1.0 trên Windows, sử dụng bản SFML dành cho MinGW-w64 64-bit.  
  - Đảm bảo đã tải và giải nén SFML đúng phiên bản.  

> ⚠️ Trong `Makefile`, đường dẫn mặc định là:  
> `D:/SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.2`  
> Nếu bạn cài SFML ở vị trí khác, hãy sửa lại **2 biến** trong Makefile:  

```make
CXXFLAGS = -std=c++17 -I<đường_dẫn_tới_SFML/include>
LDFLAGS  = -L<đường_dẫn_tới_SFML/lib> -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

Ví dụ trên Windows:
```make
CXXFLAGS = -std=c++17 -ID:/libs/SFML-2.6.2/include
LDFLAGS  = -LD:/libs/SFML-2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

Ví dụ trên Linux (đã cài qua apt):
```make
CXXFLAGS = -std=c++17 -I/usr/include
LDFLAGS  = -L/usr/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

---

## 📂 Cấu trúc thư mục

```
ThePowerofOnePerson/
│
├── src/                          # Mã nguồn trò chơi
│   ├── CaroGame.cpp              # File main
│   ├── Page.cpp / Page.h         # Lớp cơ sở cho các màn hình
│   ├── Home.cpp / Home.h         # Giao diện trang chủ
│   ├── ClassicModeMenu.cpp       # Giao diện chế độ cổ điển
│   ├── BombMode.cpp              # Giao diện chế độ bom
│   ├── Board.cpp / Board.h       # Bàn cờ PvP
│   ├── BoardAI.cpp / BoardAI.h   # Bàn cờ PvE (AI)
│   ├── bombBoard.cpp / bombBoard.h # Bàn cờ Bom
│   ├── bot.cpp / bot.h           # AI robot
│   ├── assets/                   # Hình ảnh, âm thanh, font
│   └── Makefile                  # Script biên dịch
│
├── docs/                         # Tài liệu & media minh hoạ
|   ├── UML.png                   # Sơ đồ lớp
│   ├── report.pdf                # Báo cáo đồ án
│   └── Link video giới thiệu sản phẩm.docx # Video demo trò chơi
│
└── README.md                     # Hướng dẫn sử dụng / cài đặt
```

---

## ⚙️ Cách biên dịch

Trong thư mục gốc của dự án, mở terminal (hoặc MSYS2 MinGW64 shell trên Windows) và chạy:

```bash
make
```

Lệnh này sẽ biên dịch tất cả file `.cpp` và tạo ra file thực thi **main.exe**.

---

## ▶️ Cách chạy chương trình

Sau khi build thành công, chạy game bằng:

```bash
./main.exe
```

---

## 🧹 Dọn dẹp

Để xóa file đối tượng `.o` và file thực thi `.exe`, chạy:

```bash
make clean
```

---

## ⚠️ Lưu ý quan trọng (Windows)

Để **main.exe** chạy được, cần có các thư viện động **.dll** của SFML và OpenAL nằm cùng thư mục với `main.exe`, hoặc nằm trong thư mục được khai báo trong biến môi trường **PATH**.

Các file cần thiết:  

- `openal32.dll`  
- `sfml-audio-2.dll`  
- `sfml-graphics-2.dll`  
- `sfml-system-2.dll`  
- `sfml-window-2.dll`  

Nếu không có, khi chạy sẽ bị lỗi “missing DLL”.


