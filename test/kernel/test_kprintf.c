#include "kprintf.h"
#include "test.h"

static void test_kprintf_basic() {
  kprintf(TEST_EXPECTED "A | hello | -123 | 123 | abc | 0x1234 | %%\r\n");
  kprintf(TEST_ACTUAL "%c | %s | %d | %u | %x | %p | %%\r\n", 'A', "hello", -123, 123U, 0xabcU,
          (void *)0x1234);
}

static void test_kprintf_long() {
  kprintf(TEST_EXPECTED "-12345678900 | 12345678900 | abcdef1234\r\n");
  kprintf(TEST_ACTUAL "%ld | %lu | %lx\r\n", -12345678900L, 12345678900UL, 0xabcdef1234UL);
}

static void test_kprintf_width() {
  kprintf(TEST_EXPECTED "<   A> | <hello   > | <   123> | <abc   >\r\n");
  kprintf(TEST_ACTUAL "<%4c> | <%-8s> | <%*d> | <%*x>\r\n", 'A', "hello", 6, 123, -6, 0xabcU);
}

void test_kprintf() {
  test_kprintf_basic();
  test_blank_line();

  test_kprintf_long();
  test_blank_line();

  test_kprintf_width();
  test_blank_line();
}
