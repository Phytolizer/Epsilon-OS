#include "terminal.h"
#include "memory.h"

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
  if (c == '\n') {
    terminal->column = 0;
    ++terminal->row;
    if (terminal->row == VGA_HEIGHT) {
      terminal->row = 0;
    }
  } else {
    terminal_put_entry_at(terminal, c, terminal->color, terminal->column,
                          terminal->row);
    ++terminal->column;
    if (terminal->column == VGA_WIDTH) {
      terminal->column = 0;
      ++terminal->row;
      if (terminal->row == VGA_HEIGHT) {
        terminal_scroll(terminal, 1);
      }
    }
  }
}

void terminal_write(struct terminal *terminal, struct string_view sv) {
  for (size_t i = 0; i < sv.length; ++i) {
    terminal_put_char(terminal, sv.data[i]);
  }
}

void terminal_scroll(struct terminal *terminal, long amount) {
  if (amount == 0) {
    return;
  }
  if (amount > 0) {
    for (long i = amount; i < VGA_HEIGHT; ++i) {
      memcpy(&terminal->buffer[(i - amount) * VGA_WIDTH],
             &terminal->buffer[i * VGA_WIDTH], VGA_WIDTH * sizeof(uint16_t));
    }
    for (long i = VGA_HEIGHT - amount; i < VGA_HEIGHT; ++i) {
      memset(&terminal->buffer[i * VGA_WIDTH], 0, VGA_WIDTH * sizeof(uint16_t));
    }
  } else {
    for (long i = VGA_HEIGHT - amount - 1; i >= 0; ++i) {
      memcpy(&terminal->buffer[(i + amount) * VGA_WIDTH],
             &terminal->buffer[i * VGA_WIDTH], VGA_WIDTH * sizeof(uint16_t));
    }
    for (long i = 0; i < amount; ++i) {
      memset(&terminal->buffer[i * VGA_WIDTH], 0, VGA_WIDTH * sizeof(uint16_t));
    }
  }
}
