#pragma once

#include "string.h"
#include "vga.h"
#include <stddef.h>
#include <stdint.h>

struct terminal {
  size_t row;
  size_t column;
  uint8_t color;
  uint16_t *buffer;
};

void terminal_init(struct terminal *terminal);
void terminal_set_color(struct terminal *terminal, uint8_t color);
void terminal_put_entry_at(struct terminal *terminal, char c, uint8_t color,
                           size_t x, size_t y);
void terminal_put_char(struct terminal *terminal, char c);
void terminal_write(struct terminal *terminal, struct string_view sv);
