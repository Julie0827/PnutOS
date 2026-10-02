#pragma once

#include <stdint.h>

void memory_init(uint64_t base, uint64_t size);
void memory_print_info(const char *prefix);
