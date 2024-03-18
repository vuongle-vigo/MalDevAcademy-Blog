## Syscalls - Custom APC Injection with Syscalls

Ở bài viết trước đã thảo luận về [API Hooking](https://github.com/vuongle-vigo/MalDevAcademy-Blog/tree/main/14-3-2024%20API%20Hooking), có thể sử dụng kĩ thuật này để phát hiện payload được cấp quyền bởi `VirtualProtectEx` ở cuối bài viết. Mình cũng đã có 1 bài viết về việc bypass kĩ thuật Hooking này bằng [Direct Syscall](https://github.com/vuongle-vigo/WinMalHack-Blog/tree/main/Process%20Injection/Bypass%20AV%20Hook%20-%20Direct%20Syscall).

Bài viết này không thực hiện theo MalDevAcademy. 

### Build syscall code with gcc/g++

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

### Custom APC Injection with syscall
