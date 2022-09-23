#pragma once

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

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t vga_entry(char c, uint8_t color);

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
