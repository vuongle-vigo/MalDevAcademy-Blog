### Setup gcc để build file exe thay vì sử dụng VS2022

- Link tải: https://winlibs.com/
- Cmd: gcc main.cpp -o main.exe -lntdll -lkernel32 -O2 -DNDEBUG	-s			//-O2 -O3 mức độ tối ưu code, -DNDEBUG bỏ symbol debug, -s làm gọn code (chưa rõ)