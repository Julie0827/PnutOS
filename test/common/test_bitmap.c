#include "bitmap.h"
#include "kassert.h"
#include "test.h"

#include <stdint.h>

static void test_bitmap_set() {
  uint8_t bitmap[2] = {0};

  bitmap_set(bitmap, 0);
  KASSERT(bitmap[0] == 0x01);

  bitmap_set(bitmap, 3);
  KASSERT(bitmap[0] == 0x09);

  bitmap_set(bitmap, 7);
  KASSERT(bitmap[0] == 0x89);

  bitmap_set(bitmap, 3);
  KASSERT(bitmap[0] == 0x89);

  bitmap_set(bitmap, 8);
  KASSERT(bitmap[1] == 0x01);

  bitmap_set(bitmap, 15);
  KASSERT(bitmap[1] == 0x81);
}

static void test_bitmap_clear() {
  uint8_t bitmap[2] = {0xFF, 0xFF};

  bitmap_clear(bitmap, 0);
  KASSERT(bitmap[0] == 0xFE);

  bitmap_clear(bitmap, 3);
  KASSERT(bitmap[0] == 0xF6);

  bitmap_clear(bitmap, 7);
  KASSERT(bitmap[0] == 0x76);

  bitmap_clear(bitmap, 3);
  KASSERT(bitmap[0] == 0x76);

  bitmap_clear(bitmap, 8);
  KASSERT(bitmap[1] == 0xFE);

  bitmap_clear(bitmap, 15);
  KASSERT(bitmap[1] == 0x7E);
}

static void test_bitmap_test() {
  uint8_t bitmap[2] = {0x89, 0x76};

  KASSERT(bitmap_test(bitmap, 3));
  KASSERT(!bitmap_test(bitmap, 4));
  KASSERT(bitmap_test(bitmap, 7));

  KASSERT(bitmap_test(bitmap, 9));
  KASSERT(!bitmap_test(bitmap, 11));
  KASSERT(bitmap_test(bitmap, 13));
}

void test_bitmap() {
  test_bitmap_set();
  test_bitmap_clear();
  test_bitmap_test();
}
