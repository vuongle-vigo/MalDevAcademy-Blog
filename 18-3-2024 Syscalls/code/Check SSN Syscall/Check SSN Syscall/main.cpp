# include "common.h"
# include "NtFunction.hpp"
# include "Hashing.hpp"

EXTERN_C VOID HellsGate(DWORD dwSSN);
EXTERN_C NTSTATUS HellDescent(...);

int main() {
	unsigned __int64 i;
	i = GetLastError();
	i = SetCriticalSectionSpinCount(NULL, NULL);
	i = GetWindowContextHelpId(NULL);
	i = GetWindowLongPtrW(NULL, NULL);
	i = RegisterClassW(NULL);
	i = IsWindowVisible(NULL);
	i = ConvertDefaultLocale(NULL);
	i = MultiByteToWideChar(NULL, NULL, NULL, NULL, NULL, NULL);
	i = IsDialogMessageW(NULL, NULL);
	int data = 0xabcd;
	NT_API_TABLE NtApiTable;
	BYTE buffer[8] = { 0 };
	HANDLE hProc = GetCurrentProcess();
	GetNtApiTableInfo(&NtApiTable);
	HellsGate(NtApiTable.NtReadVirtualMemory.dwSSN);
	PVOID address = hProc;
	NTSTATUS x = HellDescent(hProc, &data, buffer, 8, NULL);
	//BOOL a = ReadProcessMemory(hProc, &address, buffer, 8, NULL);
	return 1;
}