#pragma once

#include <stddef.h>
#include <stdint.h>

void bitmap_set(uint8_t *bitmap, size_t index);
void bitmap_clear(uint8_t *bitmap, size_t index);
bool bitmap_test(const uint8_t *bitmap, size_t index);
