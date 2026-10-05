#pragma once

#include <stdint.h>

void memory_init(uint64_t base, uint64_t size);
uint64_t memory_get_end();
void memory_print_info(const char *prefix);
