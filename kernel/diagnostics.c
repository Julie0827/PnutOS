#include "diagnostics.h"
#include "config.h"
#include "kprintf.h"

extern char kernel_start[];
extern char kernel_end[];

extern char stack_bottom[];
extern char stack_top[];

static const char boot_prefix[] = "[boot] ";
static const char diag_prefix[] = "[diag] ";

void diagnostics_print(unsigned long hart_id, struct dtb_memory memory) {
#if CONFIG_DIAGNOSTICS
  kprintf("%shart: %lu\n", boot_prefix, hart_id);

  kprintf("%skernel: %p - %p\n", diag_prefix, kernel_start, kernel_end);
  kprintf("%sstack: %p - %p\n", diag_prefix, stack_bottom, stack_top);
  kprintf("%sram: %p - %p (%lu MiB)\n", diag_prefix, (void *)(uintptr_t)memory.base,
          (void *)(uintptr_t)(memory.base + memory.size), memory.size / (1024 * 1024));
#endif
}
