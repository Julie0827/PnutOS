#pragma once

#include <stdint.h>

struct dtb_memory {
  uint64_t base;
  uint64_t size;
};

struct dtb_info {
  struct dtb_memory memory;
  uint32_t timebase_frequency;
};

struct dtb_info dtb_parse(const void *dtb);
