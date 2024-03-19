#include "sysfn032.h"

BOOL Rc4EncryptionViaSystemFunc032(IN PBYTE pRc4Key, IN PBYTE pPayloadData, IN DWORD dwRc4KeySize, IN DWORD sPayloadSize) {

	NTSTATUS STATUS = NULL;

	USTRING Data = {
		.Buffer = pPayloadData,
		.Length = sPayloadSize,
		.MaximumLength = sPayloadSize
	};

	USTRING	Key = {
		.Buffer = pRc4Key,
		.Length = dwRc4KeySize,
		.MaximumLength = dwRc4KeySize
	};

	//Can call SystemFunction033 like SystemFunction032
	fnSystemFunction032 SystemFunction032 = (fnSystemFunction032)GetProcAddress(LoadLibraryA("Advapi32"), "SystemFunction032");

	if ((STATUS = SystemFunction032(&Data, &Key)) != 0x0) {
		printf("[!] SystemFunction032 FAILED With Error: 0x%0.8X \n", STATUS);
		return FALSE;
	}

	return TRUE;
}