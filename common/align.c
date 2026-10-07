#include "align.h"

uintptr_t align_up_addr(uintptr_t addr, uintptr_t alignment) {
  return (addr + alignment - 1) & ~(alignment - 1);
}

uintptr_t align_down_addr(uintptr_t addr, uintptr_t alignment) {
  return addr & ~(alignment - 1);
}

bool align_up_size_overflows(size_t size, size_t alignment) {
  return size > SIZE_MAX - alignment + 1;
}

size_t align_up_size(size_t size, size_t alignment) {
  return (size + alignment - 1) & ~(alignment - 1);
}
