#include "interrupt.h"

#include <stdint.h>

#define SSTATUS_SIE (1ULL << 1)

void interrupt_enable() {
  __asm__ volatile("csrs sstatus, %0" : : "r"(SSTATUS_SIE));
}

bool interrupt_disable() {
  uint64_t sstatus;

  __asm__ volatile("csrrc %0, sstatus, %1" : "=r"(sstatus) : "r"(SSTATUS_SIE));

  return sstatus & SSTATUS_SIE;
}

void interrupt_restore(bool was_enabled) {
  if (was_enabled) interrupt_enable();
}
