#pragma once
#pragma once

# include "common.h"

#define TOKENIZE(x) #x
#define CONCAT(X, Y) X##Y

#define HASHALGOA HashStringA
#define HASHALGOW HashStringW

constexpr int RandomCompileTimeSeed(void)
{
    return '0' * -40271 +
        __TIME__[7] * 1 +
        __TIME__[6] * 10 +
        __TIME__[4] * 60 +
        __TIME__[3] * 600 +
        __TIME__[1] * 3600 +
        __TIME__[0] * 36000;
};

constexpr auto SEED = RandomCompileTimeSeed();

constexpr unsigned long HashStringA(const char* str) {
    unsigned long hash = (unsigned long)SEED;
    int c = SEED;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

constexpr unsigned long HashStringW(const wchar_t* str)
{
    unsigned long hash = (unsigned long)SEED;
    int c = SEED;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}


#define hash(VAL) constexpr auto CONCAT(hash, VAL) = HASHALGOA(TOKENIZE(VAL));
#define hashDll(DLL, VAL) constexpr auto CONCAT(hash, DLL) = HASHALGOW(VAL);

#define hashFunc(FUNCNAME, RETTYPE, ...)\
hash(FUNCNAME) typedef RETTYPE(WINAPI* CONCAT(type, FUNCNAME))(__VA_ARGS__);

hashDll(KERNEL32, L"kernel32.dll")
hashDll(USER32, L"user32.dll")
hashDll(NTDLL, L"ntdll.dll")


hashFunc(NtReadVirtualMemory, NTSTATUS,
    _In_ HANDLE ProcessHandle,
    _In_opt_ PVOID BaseAddress,
    _Out_writes_bytes_(BufferSize) PVOID Buffer,
    _In_ SIZE_T BufferSize,
    _Out_opt_ PSIZE_T NumberOfBytesRead
)

void toLower(PWCHAR dest, PWCHAR source);

