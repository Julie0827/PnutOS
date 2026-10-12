#include "trap.h"

void trap_handler(struct trap_frame *frame) {
  (void)frame;

  while (1) {
    __asm__ volatile("wfi");
  }
}
