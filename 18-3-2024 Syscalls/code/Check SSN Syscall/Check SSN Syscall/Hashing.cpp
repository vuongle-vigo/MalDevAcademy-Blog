#include "Hashing.hpp"

void toLower(PWCHAR dest, PWCHAR source) {
    int i = 0;
    while (source[i]) {
        if (source[i] > 'A' && source[i] < 'Z') {
            dest[i] = source[i] + 32;
        }
        else {
            dest[i] = source[i];
        }
        //dest[i] = (WCHAR)tolower(source[i]);
        i++;
    }
}