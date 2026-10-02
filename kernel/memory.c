#include "memory.h"
#include "kprintf.h"

extern char kernel_start[];
extern char kernel_end[];

extern char stack_bottom[];
extern char stack_top[];

struct memory_info {
  uint64_t base;
  uint64_t size;
};

static struct memory_info memory;

void memory_init(uint64_t base, uint64_t size) {
  memory.base = base;
  memory.size = size;
}

void memory_print_info(const char *prefix) {
  kprintf("%skernel: %p - %p\n", prefix, kernel_start, kernel_end);
  kprintf("%sstack: %p - %p\n", prefix, stack_bottom, stack_top);
  kprintf("%sram: %p - %p (%lu MiB)\n", prefix, (void *)(uintptr_t)memory.base,
          (void *)(uintptr_t)(memory.base + memory.size), memory.size / (1024 * 1024));
}
