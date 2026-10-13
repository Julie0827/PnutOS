#include "panic.h"
#include "ansi.h"
#include "cpu.h"
#include "kprintf.h"

[[noreturn]] void panic(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  kprintf(ANSI_RED "PANIC: ");
  vkprintf(fmt, args);
  kprintf(ANSI_RESET "\r\n");

  va_end(args);

  cpu_halt();
}
