# include <Windows.h>
# include <stdio.h>
# include <winternl.h>
# include "HashString.h"

typedef int (*WINAPI MessageBoxType) (
	HWND   hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT   uType
	);

void toLower(PWCHAR dest, PWCHAR source) {
	int i = 0;
	while (source[i]) {
		dest[i] = (WCHAR)tolower(source[i]);
		i++;
	}
}

FARPROC GetProcAddressReplacement(IN HMODULE hModule, IN unsigned long hashFunc) {
	PBYTE pBase = (PBYTE)hModule;
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pBase;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE) {
		return NULL;
	}

	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pBase + pDos->e_lfanew);
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

		if (HashStringA(functionName) == hashFunc) {
			return (FARPROC)functionAddress;
		}
	}

	return NULL;
}

HMODULE GetModuleHandleReplacement(IN unsigned long hashDll) {
#ifdef _WIN64
	PPEB pPeb = (PEB*)(__readgsqword(0x60));
#elif _WIN32 
	PPEB pPeb = (PEB*)(__readfsdword(0x30));
#endif

	LIST_ENTRY header = pPeb->Ldr->InMemoryOrderModuleList;
	PLIST_ENTRY pListEntry = header.Flink;
	PLDR_DATA_TABLE_ENTRY ldrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)(pListEntry);
	while (ldrDataTableEntry->FullDllName.Buffer) {
		WCHAR tmp[50] = { 0 };
		toLower(tmp, ldrDataTableEntry->FullDllName.Buffer);
		if (hashDll == HashStringW(tmp)) {
			return HMODULE(ldrDataTableEntry->Reserved2[0]);
		}
		pListEntry = pListEntry->Flink;
		ldrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)(pListEntry);
	}
	return (HMODULE)NULL;
}


int main() {
	LoadLibraryA("user32.dll");
	HMODULE hUser32 = NULL;
	hUser32 = GetModuleHandleReplacement(hashUSER32);
	if (hUser32) {
		MessageBoxType MessageBoxfn = (MessageBoxType)GetProcAddressReplacement(hUser32, hashMessageBoxA);
		MessageBoxfn(NULL, "Success", "Notification", NULL);
	}

	

	return 1;
}