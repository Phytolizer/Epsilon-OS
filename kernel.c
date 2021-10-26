#include "my_libc/string.h"
#include "my_libc/terminal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str) {
  size_t len = 0;
  for (const char *p = str; *p != '\0'; ++p) {
    ++len;
  }
  return len;
}

void kernel_main(void) {
  struct terminal terminal;
  terminal_init(&terminal);
  terminal_write(&terminal, STRING_VIEW_LIT("Hello, Kernel!\n"));
}
