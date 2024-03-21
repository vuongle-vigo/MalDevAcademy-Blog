#pragma once
#include <windows.h>



typedef struct _USTRING
{
	DWORD	Length;         // Size of the data to encrypt/decrypt
	DWORD	MaximumLength;  // Max size of the data to encrypt/decrypt, although often its the same as Length (USTRING.Length = USTRING.MaximumLength = X)
	PVOID	Buffer;         // The base address of the data to encrypt/decrypt
} USTRING;

typedef NTSTATUS(NTAPI* fnSystemFunction032)(
	USTRING* Data,   // Structure of type USTRING that holds information about the buffer to encrypt / decrypt
	USTRING* Key     // Structure of type USTRING that holds information about the key used while encryption / decryption
	);
/*
Helper function that calls SystemFunction032
* pRc4Key - The RC4 key use to encrypt/decrypt
* pPayloadData - The base address of the buffer to encrypt/decrypt
* dwRc4KeySize - Size of pRc4key (Param 1)
* sPayloadSize - Size of pPayloadData (Param 2)
*/
BOOL Rc4EncryptionViaSystemFunc032(IN PBYTE pRc4Key, IN PBYTE pPayloadData, IN DWORD dwRc4KeySize, IN DWORD sPayloadSize);


