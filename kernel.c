#include "tools/string.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum vga_color {
  vga_color_black,
  vga_color_blue,
  vga_color_green,
  vga_color_cyan,
  vga_color_red,
  vga_color_magenta,
  vga_color_brown,
  vga_color_light_grey,
  vga_color_dark_grey,
  vga_color_light_blue,
  vga_color_light_green,
  vga_color_light_cyan,
  vga_color_light_red,
  vga_color_light_magenta,
  vga_color_light_brown,
  vga_color_white,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(char c, uint8_t color) {
  return (uint16_t)c | (uint16_t)color << 8;
}

size_t strlen(const char *str) {
  size_t len = 0;
  for (const char *p = str; *p != '\0'; ++p) {
    ++len;
  }
  return len;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

struct terminal {
  size_t row;
  size_t column;
  uint8_t color;
  uint16_t *buffer;
};

void terminal_init(struct terminal *terminal) {
  terminal->row = 0;
  terminal->column = 0;
  terminal->color = vga_entry_color(vga_color_light_grey, vga_color_black);
  terminal->buffer = (uint16_t *)0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; ++y) {
    for (size_t x = 0; x < VGA_WIDTH; ++x) {
      size_t index = x + y * VGA_WIDTH;
      terminal->buffer[index] = vga_entry(' ', terminal->color);
    }
  }
}

void terminal_set_color(struct terminal *terminal, uint8_t color) {
  terminal->color = color;
}

void terminal_put_entry_at(struct terminal *terminal, char c, uint8_t color,
                           size_t x, size_t y) {
  size_t index = x + y * VGA_WIDTH;
  terminal->buffer[index] = vga_entry(c, color);
}

void terminal_put_char(struct terminal *terminal, char c) {
  terminal_put_entry_at(terminal, c, terminal->color, terminal->column,
                        terminal->row);
  ++terminal->column;
  if (terminal->column == VGA_WIDTH) {
    terminal->column = 0;
    ++terminal->row;
    if (terminal->row == VGA_HEIGHT) {
      terminal->row = 0;
    }
  }
}

void terminal_write(struct terminal *terminal, struct string_view sv) {
  for (size_t i = 0; i < sv.length; ++i) {
    terminal_put_char(terminal, sv.data[i]);
  }
}

void kernel_main(void) {
  struct terminal terminal;
  terminal_init(&terminal);
  terminal_write(&terminal, STRING_VIEW_LIT("Hello, Kernel!\n"));
}
