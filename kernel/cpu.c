#include "cpu.h"

[[noreturn]] void cpu_halt() {
  while (1) {
    __asm__ volatile("wfi");
  }
}
