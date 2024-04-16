## Syscalls 

Ở bài viết trước đã thảo luận về [API Hooking](https://github.com/vuongle-vigo/MalDevAcademy-Blog/tree/main/14-3-2024%20API%20Hooking), có thể sử dụng kĩ thuật này để phát hiện payload được cấp quyền bởi `VirtualProtectEx` ở cuối bài viết. Mình cũng đã có 1 bài viết về việc bypass kĩ thuật Hooking này bằng [Direct Syscall](https://github.com/vuongle-vigo/WinMalHack-Blog/tree/main/Process%20Injection/Bypass%20AV%20Hook%20-%20Direct%20Syscall).


### Xây dựng syscall code asm với gcc/g++

Trong bài viết [Direct Syscall](https://github.com/vuongle-vigo/WinMalHack-Blog/tree/main/Process%20Injection/Bypass%20AV%20Hook%20-%20Direct%20Syscall) đã đề cập tới việc sử dụng vs2022/masm để build mã syscall, tuy nhiên như các bài viết trước, mã build với vs2022 dễ bị phát hiện bởi AV, vì vậy bài viết này sẽ hướng dẫn build mã syscall bằng gcc/g++.

Điểm khác biệt ở 2 trình compile này là gcc/g++ sử dụng định danh GAS thay vì MASM, vì vậy mã asm sẽ cần viết khác 1 chút. 

***MASM***
```asm
.code
SysNtReadVirtualMemory proc
    mov     r10, rcx
    mov     eax, 3Fh
    syscall
    ret
SysNtReadVirtualMemory endp
end
```

***GAS***
```s
.global SysNtReadVirtualMemory

SysNtReadVirtualMemory:
    mov     %rcx, %r10
    mov     $0x3F, %eax
    syscall
    ret
```

Khai báo `global` để có thể sử dụng từ bên ngoài, sử dụng % trước thanh ghi, $ trước tiền tố và cú pháp `source, des`. Mã `GAS` cần phải được lưu file .s

***Compile***

Đầu tiên cần chuyển đổi file .s thành object .o:

```
gcc -c syscall.s -o syscall.o
```

Sau đó biên dịch với file .c/.cpp đi kèm:

```
gcc main.c syscall.o -o main.exe
```

Với việc xây dựng mã bằng phương pháp trên, chúng ta có thể thấy rằng trong mã sẽ xuất hiện nhiều phần mã asm lặp đi lặp lại, dẫn đến việc AV có thể phát hiện. Điều này đã được chứng minh trong bài [Bypass AV](https://github.com/vuongle-vigo/MalDevAcademy-Blog/tree/main/20-3-2024%20Bypass%20AV).

Giải pháp đề ra là sẽ thực hiện sử dụng chỉ duy nhất 1 mã gọi Syscall asm với mã NTAPI (SSN) chưa được xác định, ta sẽ thực hiện đi tính toán mã của NTAPI cần gọi để tìm ra SSN, sau đó thay thế vào asm rồi thực hiện gọi như trước.

### Tìm SSN của NTAPI

Như đã biết ở trước, để lấy được địa chỉ thực thi của hàm NT, cách đơn giản nhất đó là sử dụng GetProcAddress.
```c
# include <Windows.h>

int main() {
    HMODULE hNtdll = LoadLibraryA("ntdll.dll");
    FARPROC address = GetProcAddress(hNtdll, "NtReadVirtualMemory");

    return 1;
}
```

Debug + Dissassembly để xem mã tại `address` được trả về từ `GetProcAddress` ở trên, nhận được kết quả sau:

![ntapi1](images/ntapi1.png)

Vậy có thể thấy địa chỉ trả về của `GetProcAddress` chính là phần mã của NTAPI, để lấy được SSN, chỉ cần cộng thêm 4 bytes và đọc giá trị tại đó.

```c
BYTE* SSNptr = (BYTE*)address + 4;
DWORD SSN = *SSNptr;
```

Đây chính là giá trị của SSN mà chúng ta lấy được. Tiếp theo làm sao để sử dụng SSN này cho lệnh gọi syscall. 

Sử dụng 2 hàm asm để thực hiện việc này, hàm thứ nhất sử dụng để lưu trữ giá trị SSN được truyển vào ecx, lưu trữ vào 1 biến global. Sau đó Hàm thứ 2 sẽ thực hiện sử dụng biến này là giá trị SSN, thực hiện gọi syscall. 

Mã sử dụng kết hợp Customer GetProcAddress, GetModuleHandle và Hashing API được đính kèm.

### Ý tưởng xây dựng indirect syscall

Với kĩ thuật tương tự như trên, hoàn toàn có thể thực hiện indirect syscall. Chúng ta sẽ cần thêm 1 biến global để lưu trữ địa chỉ của syscall của 1 hàm bất kì, sau đó thay thế lệnh syscall trong mã gốc bằng lệnh jmp đến address này. Ngoài ra, việc sử dụng các lệnh như `mov r10, rcx` hay `mov eax, SSN` sẽ khiến cho nó có signature (mình thường sử dụng để tìm kiếm SSN), giúp cho  AV tìm ra được, vì vậy có thể kết hợp các lệnh tính toán vô hại khác, làm rối mã asm để tăng khả năng né tránh AV. 