#include "memory.h"

void memcpy(void *dest, void *src, size_t count) {
  char *destc = dest;
  char *srcc = src;

  for (size_t i = 0; i < count; ++i) {
    destc[i] = srcc[i];
  }
}

void memset(void *dest, char value, size_t count) {
  char *destc = dest;
  for (size_t i = 0; i < count; ++i) {
    destc[i] = value;
  }
}
