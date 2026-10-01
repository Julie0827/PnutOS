#include "panic.h"
#include "kprintf.h"

[[noreturn]] void panic(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  kprintf("PANIC: ");
  vkprintf(fmt, args);
  kprintf("\r\n");

  va_end(args);

  while (1) {
  }
}
