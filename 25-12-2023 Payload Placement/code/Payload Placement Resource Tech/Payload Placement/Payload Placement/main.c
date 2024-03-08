#include <stdio.h>
#include <Windows.h>
#include "resource.h"

#define DEBUG(x, ...) printf(x, ##__VA_ARGS__)

int main() {
HRSRC		hRsrc = NULL;
HGLOBAL		hGlobal = NULL;
PVOID		pPayload = NULL;
SIZE_T		sPayloadSize = NULL;


hRsrc = FindResourceW(NULL, MAKEINTRESOURCEW(IDR_RCDATA1), RT_RCDATA);
if (hRsrc == NULL) {
	DEBUG("[!] FindResourceW Failed With Error : %d \n", GetLastError());
	return -1;
}

hGlobal = LoadResource(NULL, hRsrc);
if (hGlobal == NULL) {
	DEBUG("[!] LoadResource Failed With Error : %d \n", GetLastError());
	return -1;
}

pPayload = LockResource(hGlobal);
if (pPayload == NULL) {
	DEBUG("[!] LockResource Failed With Error : %d \n", GetLastError());
	return -1;
}

sPayloadSize = SizeofResource(NULL, hRsrc);
if (sPayloadSize == NULL) {
	DEBUG("[!] SizeofResource Failed With Error : %d \n", GetLastError());
	return -1;
}

printf("[i] Payload address : 0x%p \n", pPayload);
printf("[i] sPayloadSize var : %ld \n", sPayloadSize);
printf("[#] Press <Enter> To Quit ...");
getchar();
return 0;
}
