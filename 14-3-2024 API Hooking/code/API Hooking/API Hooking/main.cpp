# include <Windows.h>
# include <iostream>

#ifdef _M_X64 
#define PATCH_SIZE 13           //if compiling as 64 bits
#endif
#ifdef  M_IX86                  //if compiling as 32 bits
#define PATCH_SIZE 7
#endif

typedef struct _HookSt {
    PVOID pFunction2Hook;               //address of function to hook
    PVOID pFunction2Run;                //address of function to run
    BYTE pOriginalBytes[PATCH_SIZE];    //buffer to keep original bytes of "function to hook"
    DWORD dwOldProctection;             //hold old memory protection of the "function to hook" when call VirtualProtect
} HookStr, *PHookSt;

BOOL InititalizeHookStruct(IN PVOID pFunction2Hook, IN PVOID pFunction2Run, OUT PHookSt Hook) {
    Hook->pFunction2Hook = pFunction2Hook;
    Hook->pFunction2Run = pFunction2Run;
    memcpy(Hook->pOriginalBytes, pFunction2Hook, PATCH_SIZE);
    if (!VirtualProtect(pFunction2Hook, PATCH_SIZE, PAGE_EXECUTE_READWRITE, &Hook->dwOldProctection)) {
        printf("[!] VirtualProtect Failed with Error: %d \n", GetLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL
#pragma optimize(off)

int MyMessageBoxA(_In_opt_ HWND hWnd,
    _In_opt_ LPCSTR lpText,
    _In_opt_ LPCSTR lpCaption,
    _In_ UINT uType) {
    
    MessageBoxW(hWnd, NULL, L"HEHEHEH", uType);

    return 1;
}
#pragma optimize(on)

int main() {

    BYTE patch[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
    DWORD_PTR y = (DWORD_PTR) & MyMessageBoxA;
    memcpy(patch + 2, &y, 8);
    DWORD oldProtect;
    PVOID x = &MessageBoxA;
    BYTE tmp[12] = { 0 };
    VirtualProtect(&MessageBoxA, 12, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(tmp, &MessageBoxA, 12);
    memcpy(&MessageBoxA, patch, 12);
   
	MessageBoxA(NULL, NULL, NULL, NULL);

    memcpy(&MessageBoxA, tmp, 12);
    MessageBoxA(NULL, NULL, NULL, NULL);
    return 1;
}