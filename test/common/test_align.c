#include "align.h"
#include "kassert.h"
#include "test.h"

static void test_align_up_addr() {
  KASSERT(align_up_addr(0, 4096) == 0);
  KASSERT(align_up_addr(0x1000, 4096) == 0x1000);
  KASSERT(align_up_addr(0x1001, 4096) == 0x2000);
  KASSERT(align_up_addr(0x1FFF, 4096) == 0x2000);

  KASSERT(align_up_addr(123, 1) == 123);
  KASSERT(align_up_addr(13, 8) == 16);
  KASSERT(align_up_addr(17, 16) == 32);
  KASSERT(align_up_addr(100, 64) == 128);
}

static void test_align_down_addr() {
  KASSERT(align_down_addr(0, 4096) == 0);
  KASSERT(align_down_addr(0x1000, 4096) == 0x1000);
  KASSERT(align_down_addr(0x1001, 4096) == 0x1000);
  KASSERT(align_down_addr(0x1FFF, 4096) == 0x1000);

  KASSERT(align_down_addr(123, 1) == 123);
  KASSERT(align_down_addr(13, 8) == 8);
  KASSERT(align_down_addr(17, 16) == 16);
  KASSERT(align_down_addr(100, 64) == 64);
}

static void test_align_up_size_overflows() {
  KASSERT(!align_up_size_overflows(0, 4096));
  KASSERT(!align_up_size_overflows(4096, 4096));
  KASSERT(!align_up_size_overflows(4097, 4096));

  KASSERT(!align_up_size_overflows(SIZE_MAX - 4095, 4096));
  KASSERT(align_up_size_overflows(SIZE_MAX - 4094, 4096));
  KASSERT(align_up_size_overflows(SIZE_MAX, 4096));

  KASSERT(!align_up_size_overflows(SIZE_MAX, 1));
  KASSERT(!align_up_size_overflows(SIZE_MAX - 7, 8));
  KASSERT(align_up_size_overflows(SIZE_MAX - 6, 8));
}

static void test_align_up_size() {
  KASSERT(align_up_size(0, 4096) == 0);
  KASSERT(align_up_size(4096, 4096) == 4096);
  KASSERT(align_up_size(4097, 4096) == 8192);
  KASSERT(align_up_size(8191, 4096) == 8192);

  KASSERT(align_up_size(123, 1) == 123);
  KASSERT(align_up_size(13, 8) == 16);
  KASSERT(align_up_size(17, 16) == 32);
  KASSERT(align_up_size(100, 64) == 128);

  KASSERT(align_up_size(SIZE_MAX - 4095, 4096) == SIZE_MAX - 4095);
}

void test_align() {
  test_align_up_addr();
  test_align_down_addr();
  test_align_up_size_overflows();
  test_align_up_size();
}
