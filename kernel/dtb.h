#pragma once

#include <stdint.h>

struct dtb_memory {
  uint64_t base;
  uint64_t size;
};

struct dtb_memory dtb_get_memory(const void *dtb);
