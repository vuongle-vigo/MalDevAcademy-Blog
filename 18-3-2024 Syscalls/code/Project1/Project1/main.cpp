# include <string>
# include <iostream>
# include <Windows.h>

typedef struct _HeapDataStruct {
    HANDLE m_hHeap;
    LPVOID m_pReturnHeap;
} HeapDataStruct, * PHeapDataStruct;

unsigned long long pow(int base, int exp) {
    unsigned long long result = 1;
    for (int i = 0; i < exp; i++) {
        result = result * base;
    }
    return result;
}


PHeapDataStruct intToCharPtr(unsigned long long value) {
    HeapDataStruct heapDataStruct = { 0 };
    int length = 1;
    while (value / pow(10, length) != 0) {
        length++;
    }
    
    heapDataStruct.m_hHeap = HeapCreate(0, length, 0);
    if (heapDataStruct.m_hHeap == NULL) {
        return nullptr;
    }
    heapDataStruct.m_pReturnHeap = HeapAlloc(heapDataStruct.m_hHeap, HEAP_ZERO_MEMORY, (length + 1) * sizeof(char));
    //add 1 for '/0' end of string
    //char* str = (char*)VirtualAlloc(NULL, (length+1) * sizeof(char), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (heapDataStruct.m_pReturnHeap == nullptr) {
        HeapDestroy(heapDataStruct.m_hHeap);
        return nullptr; 
    }
    char* str = (char*)heapDataStruct.m_pReturnHeap;
    char* p = str + length; // start from end of string
    *p = '\0'; // add null to end of string

    if (value == 0) {
        *--p = '0'; // Handle case number 0
    }
    else {
        while (value != 0) {
            *--p = '0' + (value % 10);
            value /= 10; 
        }
    }

    if (str == NULL) {
        HeapFree(heapDataStruct.m_hHeap, 0, str);
        HeapDestroy(heapDataStruct.m_hHeap);
        //VirtualFree(str, (length + 1) * sizeof(char), MEM_DECOMMIT);
        return NULL;
    }
    return &heapDataStruct;
}

int strlen_v(IN const char* src) {
    const char* p = src;
    int len = 0;
    while (*p != '\0') {
        p++;
        len++;
    }
    return len;
}

char* strcat_v(IN const char* src1, IN const char* src2) {
    int len = strlen_v(src1) + strlen_v(src2);
    char* str = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (len + 1) * sizeof(char));
    if (str == nullptr) {
        return nullptr;
    }
    char* p = str;
    for (int i = 0; i < len; i++) {
        if (i < strlen_v(src1)) {
            *p = src1[i];
        }
        else {
            *p = src2[i - strlen_v(src1)];
        }
        p++;
    }
    *p = '\0';
    return str;
}

int main() {
    unsigned long long value = 123123214123456;
    //PHeapDataStruct pHeapDataStruct = intToCharPtr(value);
    //printf("%s:%d", pHeapDataStruct->m_pReturnHeap, strlen((char*)pHeapDataStruct->m_pReturnHeap));
    const char* x = "vuongle";
    printf("%s", strcat_v(x, ":"));
    //if (str != nullptr) {
    //    std::cout << str << std::endl;
    //    free(str); // ??ng qu�n gi?i ph�ng b? nh? khi kh�ng c?n n?a
    //}
    //else {
    //    std::cout << "C?p ph�t b? nh? th?t b?i." << std::endl;
    //}
    return 0;
}