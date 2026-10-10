#include "kassert.h"
#include "string.h"
#include "test.h"

#include <stddef.h>
#include <stdint.h>

static void test_strlen() {
  KASSERT(strlen("") == 0);
  KASSERT(strlen("a") == 1);
  KASSERT(strlen("hello") == 5);
  KASSERT(strlen("hello world") == 11);
  KASSERT(strlen("hello\nworld") == 11);
  KASSERT(strlen("hello\tworld") == 11);
  KASSERT(strlen("hello\0world") == 5);
}

static void test_strcmp() {
  KASSERT(strcmp("", "") == 0);
  KASSERT(strcmp("a", "a") == 0);
  KASSERT(strcmp("hello", "hello") == 0);
  KASSERT(strcmp("hello\0world", "hello") == 0);

  KASSERT(strcmp("", "abc") < 0);
  KASSERT(strcmp("abc", "abd") < 0);
  KASSERT(strcmp("abc", "abcd") < 0);
  KASSERT(strcmp("hello", "hello\n") < 0);

  KASSERT(strcmp("abc", "") > 0);
  KASSERT(strcmp("abd", "abc") > 0);
  KASSERT(strcmp("abcd", "abc") > 0);
  KASSERT(strcmp("hello\n", "hello") > 0);
}

static void assert_filled(const uint8_t *buf, uint8_t val, size_t len) {
  for (size_t i = 0; i < len; i++) {
    KASSERT(buf[i] == val);
  }
}

static void test_memset() {
  uint8_t buf[4];
  size_t len = sizeof(buf);

  memset(buf, 0xFF, len);
  assert_filled(buf, 0xFF, len);

  memset(buf, 0, len);
  assert_filled(buf, 0, len);

  memset(buf, 0xAB, len);
  assert_filled(buf, 0xAB, len);

  memset(buf, 0x12, 2);
  assert_filled(buf, 0x12, 2);
  KASSERT(buf[2] == 0xAB);
  KASSERT(buf[3] == 0xAB);

  KASSERT(memset(buf + 2, 0x12, len - 2) == buf + 2);
  assert_filled(buf, 0x12, len);

  memset(buf, 0xFF, 0);
  assert_filled(buf, 0x12, len);
}

void test_string() {
  test_strlen();
  test_strcmp();
  test_memset();
}
