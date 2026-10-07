#pragma once

#include <stddef.h>
#include <stdint.h>

// All functions require alignment to be a nonzero power of two.

uintptr_t align_up_addr(uintptr_t addr, uintptr_t alignment);
uintptr_t align_down_addr(uintptr_t addr, uintptr_t alignment);

bool align_up_size_overflows(size_t size, size_t alignment);
size_t align_up_size(size_t size, size_t alignment);
