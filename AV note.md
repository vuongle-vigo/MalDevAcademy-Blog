### AV note
- Mã build: g++ main.cpp -o main.exe -lntdll -s, hoặc gcc main.cpp -o main.exe -lntdll -lstdc++
- Đối với việc build file exe bằng VS, cần tắt thông tin debug. Đã test thử khi build cùng 1 project, mặc dù comment code độc hại tuy nhiên việc phát hiện mã độc hại trong lần build đầu tiên đã làm cho những phần build code sau bị ảnh hưởng.

- Việc build cùng 1 mã nguồn process hollowing bằng vs2022 và gcc lần đầu, mẫu vs2022 bị phát hiện ngay lập tức mà không cần khởi chạy, mẫu gcc không bị phát hiện.

***Sử dụng build gcc***

- Mẫu Test1: Sử dụng đầy đủ các hàm + shellcode lỗi, Bỏ API QueueUserAPC không làm cho mã trở nên độc hại. 6/73

- Mẫu Test2: Sử dụng đầy đủ API, shellcode lỗi, không làm cho mã độc hại. 6/73

- Mẫu Test3: Processhollowing nguyên mẫu, build bằng g++ không bị bắt nếu không chạy => Bị bắt theo hành vi. 6/73, bản gốc msvc 10/73 => Sử dụng gcc sẽ khiến mẫu an toàn hơn.