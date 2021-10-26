#pragma once

#include <stddef.h>
#include <stdint.h>

void memory_copy(void *dest, void *src, size_t count);
void memory_set(void *dest, char value, size_t count);
