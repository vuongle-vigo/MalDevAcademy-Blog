#pragma once
# include "common.h"

typedef struct _NT_API_INFO {
	DWORD64 dw64Hash;
	DWORD dwSSN;
} NT_API_INFO, * PNT_API_INFO;


typedef struct _NT_API_TABLE {
	NT_API_INFO NtAllocateVirtualMemory;
	NT_API_INFO NtWriteVirtualMemory;
	NT_API_INFO NtProtectVirtualMemory;
	NT_API_INFO NtQueueApcThread;
	NT_API_INFO NtResumeThread;
} NT_API_TABLE, * PNT_API_TABLE;


BOOL GetNtApiTableInfo(PNT_API_TABLE NtApiTable);

