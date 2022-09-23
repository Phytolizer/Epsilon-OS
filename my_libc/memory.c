#include "memory.h"

void memory_copy(void* dest, void* src, size_t count) {
    char* destc = dest;
    char* srcc = src;

    for (size_t i = 0; i < count; ++i) {
        destc[i] = srcc[i];
    }
}

void memory_set(void* dest, unsigned char value, size_t count) {
    char* destc = dest;
    for (size_t i = 0; i < count; ++i) {
        destc[i] = value;
    }
}
