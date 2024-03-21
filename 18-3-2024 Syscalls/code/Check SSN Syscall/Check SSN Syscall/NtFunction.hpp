#pragma once
# include "common.h"

typedef struct _NT_API_INFO {
	DWORD64 dw64Hash;
	DWORD dwSSN;
} NT_API_INFO, * PNT_API_INFO;


typedef struct _NT_API_TABLE {
	NT_API_INFO NtReadVirtualMemory;
} NT_API_TABLE, * PNT_API_TABLE;


BOOL GetNtApiTableInfo(PNT_API_TABLE NtApiTable);

