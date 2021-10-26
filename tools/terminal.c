#include "terminal.h"

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
  if (c == '\n') {
    terminal->column = 0;
    ++terminal->row;
    if (terminal->row == VGA_HEIGHT) {
      terminal->row = 0;
    }
  } else {
    ++terminal->column;
    if (terminal->column == VGA_WIDTH) {
      terminal->column = 0;
      ++terminal->row;
      if (terminal->row == VGA_HEIGHT) {
        terminal->row = 0;
      }
    }
  }
}

void terminal_write(struct terminal *terminal, struct string_view sv) {
  for (size_t i = 0; i < sv.length; ++i) {
    terminal_put_char(terminal, sv.data[i]);
  }
}