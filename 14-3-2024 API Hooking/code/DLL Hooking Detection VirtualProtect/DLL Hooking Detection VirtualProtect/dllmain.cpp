// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"


#ifdef _M_X64 
#define PATCH_SIZE 12           //if compiling as 64 bits
#endif
#ifdef  _M_IX86                  //if compiling as 32 bits
#define PATCH_SIZE 7
#endif

typedef struct _HookSt {
    PVOID pFunction2Hook;               //address of function to hook
    PVOID pFunction2Run;                //address of function to run
    BYTE pOriginalBytes[PATCH_SIZE];    //buffer to keep original bytes of "function to hook"
    DWORD dwOldProctection;             //hold old memory protection of the "function to hook" when call VirtualProtect
} HookSt, * PHookSt;

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


BOOL InstallHook(IN PHookSt Hook) {
    DWORD_PTR addressFunction = (DWORD_PTR)Hook->pFunction2Run;
#ifdef _M_X64
    BYTE bPatch[] = {
        0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,         //mov rax, address
        0xFF, 0xE0                                                          //jmp rax
    };
    memcpy(bPatch + 2, &addressFunction, sizeof(addressFunction));
#endif
#ifdef _M_IX86
    BYTE bPatch[] = {
         0xB8, 0x00, 0x00, 0x00, 0x00,                                      //mov eax, address
         0xFF, 0xE0                                                         //jmp eax
    };
    memcpy(bPatch + 1, &addressFunction, sizeof(addressFunction));
#endif
    memcpy(Hook->pFunction2Hook, bPatch, sizeof(bPatch));
    return TRUE;
}

BOOL RemoveHook(IN PHookSt Hook) {
    DWORD dwOldProtection = NULL;
    memcpy(Hook->pFunction2Hook, Hook->pOriginalBytes, PATCH_SIZE);
    memset(Hook->pOriginalBytes, '\0', PATCH_SIZE);
    if (!VirtualProtect(Hook->pFunction2Hook, PATCH_SIZE, Hook->dwOldProctection, &dwOldProtection)) {
        printf("[!] VirtualProtect Error with code: %d\n", GetLastError());
        return FALSE;
    }
    Hook = { 0 };
    return TRUE;
}

HookSt Hook = { 0 };

BOOL MyVirtualProtectEx(
    HANDLE hProcess,
    LPVOID lpAddress,
    SIZE_T dwSize,
    DWORD  flNewProtect,
    PDWORD lpflOldProtect
) {
    if (flNewProtect == PAGE_EXECUTE_READWRITE) {
        printf("[+] Detection VirtualProtectEx with PAGE_EXECUTE_READWRITE\n");
        printf("[+] Dump memory:\n");
        BYTE* bBuffer = new BYTE(dwSize);
        if (!ReadProcessMemory(hProcess, lpAddress, bBuffer, dwSize, NULL)) {
            printf("[!] ReadProcessMemory Error with code: %d\n", GetLastError());
            goto end;
        }
        for (int i = 0; i < dwSize; i++) {
            printf("0x%02X ", bBuffer[i]);
        }
    }
    end:
    RemoveHook(&Hook);
    return VirtualProtectEx(
        hProcess,
        lpAddress,
        dwSize,
        flNewProtect,
        lpflOldProtect
    );
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: { 
        
        if (!InititalizeHookStruct((PVOID)VirtualProtectEx, (PVOID)MyVirtualProtectEx, &Hook)) {
            printf("[!] InititalizeHookStruct Failed\n");
            break;
        }
        if (!InstallHook(&Hook)) {
            printf("[!] InstallHook Failed\n");
            break;
        }
        break;
    }
       
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

