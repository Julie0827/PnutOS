#include "bitmap.h"

void bitmap_set(uint8_t *bitmap, size_t index) {
  bitmap[index / 8] |= (1U << index % 8);
}

void bitmap_clear(uint8_t *bitmap, size_t index) {
  bitmap[index / 8] &= ~(1U << index % 8);
}

bool bitmap_test(const uint8_t *bitmap, size_t index) {
  return bitmap[index / 8] & (1U << index % 8);
}
