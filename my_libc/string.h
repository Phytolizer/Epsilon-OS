#pragma once

#include <stddef.h>

struct string {
    char *data;
    size_t length;
};

struct string_view {
    const char *data;
    size_t length;
};

#define STRING_VIEW_LIT(Str) \
    (struct string_view) { \
        .data = (Str), .length = sizeof(Str) - 1 \
    }

struct string_view string_make_view(struct string str);
