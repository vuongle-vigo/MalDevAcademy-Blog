# include <Windows.h>
#include <stdio.h>


typedef int (*WINAPI MessageBoxType) (
	HWND   hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT   uType
);

FARPROC GetProcAddressReplacement(IN HMODULE hModule, IN LPCSTR lpApiName) {
	PBYTE pBase = (PBYTE)hModule;
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pBase;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE) {
		return NULL;
	}

	PIMAGE_NT_HEADERS pNt = pBase + pDos->e_lfanew;
	if (pNt->Signature != IMAGE_NT_SIGNATURE) {
		return NULL;
	}

	IMAGE_OPTIONAL_HEADER pOptional = pNt->OptionalHeader;

	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(pBase + pOptional.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD pNameApi = (PDWORD)(pBase + pExport->AddressOfNames);
	PDWORD pAddressApi = (PDWORD)(pBase + pExport->AddressOfFunctions);
	PWORD pOrdinal = (PWORD)(pBase + pExport->AddressOfNameOrdinals);

	for (int i = 0; i < pExport->NumberOfFunctions; i++) {

		PCHAR functionName = (PCHAR)(pBase + pNameApi[i]);
		WORD ordinal = pOrdinal[i];
		DWORD_PTR functionAddress = (DWORD_PTR)(pBase + pAddressApi[ordinal]);
		
		printf("[%d] Name: %s - Ordinal: %d - Address: 0x%I64x\n", i, functionName, ordinal, functionAddress);
		if (!lstrcmpA(lpApiName, functionName)) {
			return functionAddress;
		}
	}

	return NULL;
}

int main() {
	HMODULE hUser32 = LoadLibraryA("user32.dll");
	//HMODULE hUser32 = GetModuleHandleA("user32.dll");
	MessageBoxType MessageBoxfn = (MessageBoxType)GetProcAddressReplacement(hUser32, "MessageBoxA");

	MessageBoxfn(NULL, NULL, NULL, NULL);

	return 1;
}