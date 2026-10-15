#include "kassert.h"
#include "test.h"
#include "timer.h"

static void test_timer_ms() {
  uint64_t start = timer_get_ms();
  uint64_t now = start;

  while (now < start + 100) {
    now = timer_get_ms();
  }

  KASSERT(now >= start + 100);
}

void test_timer() {
  test_timer_ms();
}
