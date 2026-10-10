#include "heap.h"
#include "kassert.h"
#include "string.h"
#include "test.h"

#include <stddef.h>
#include <stdint.h>

static void test_heap_alloc() {
  enum { COUNT = 10 };

  const size_t size = 64;

  uint8_t *blocks[COUNT];

  for (size_t i = 0; i < COUNT; i++) {
    blocks[i] = kmalloc(size);

    KASSERT(blocks[i] != NULL);
    KASSERT((uintptr_t)blocks[i] % alignof(max_align_t) == 0);

    for (size_t j = 0; j < i; j++) {
      KASSERT(blocks[i] != blocks[j]);
    }

    memset(blocks[i], i, size);
  }

  for (size_t i = 0; i < COUNT; i++) {
    for (size_t j = 0; j < size; j++) {
      KASSERT(blocks[i][j] == i);
    }

    kfree(blocks[i]);
  }
}

static void test_heap_calloc() {
  const size_t count = 10;
  const size_t size = 64;

  uint8_t *block = kcalloc(count, size);

  KASSERT(block != NULL);
  KASSERT((uintptr_t)block % alignof(max_align_t) == 0);

  const size_t total = count * size;

  for (size_t i = 0; i < total; i++) {
    KASSERT(block[i] == 0);
  }

  memset(block, 0xAB, total);

  for (size_t i = 0; i < total; i++) {
    KASSERT(block[i] == 0xAB);
  }

  kfree(block);
}

static void test_heap_free() {
  const size_t size = 64;

  uint8_t *block1 = kmalloc(size);
  uint8_t *block2 = kmalloc(size);

  KASSERT(block1 != NULL);
  KASSERT(block2 != NULL);

  memset(block2, 0xAB, size);

  kfree(block1);

  uint8_t *block3 = kmalloc(size);

  KASSERT(block3 != NULL);
  KASSERT((uintptr_t)block3 % alignof(max_align_t) == 0);

  memset(block3, 0xCD, size);

  for (size_t i = 0; i < size; i++) {
    KASSERT(block2[i] == 0xAB);
    KASSERT(block3[i] == 0xCD);
  }

  kfree(block2);
  kfree(block3);
}

static void test_heap_stress() {
  enum { COUNT = 10 };

  const size_t size = 2048;
  const size_t iterations = 10000;

  void *blocks[COUNT];

  for (size_t i = 0; i < iterations; i++) {
    for (size_t j = 0; j < COUNT; j++) {
      blocks[j] = kmalloc(size);

      KASSERT(blocks[j] != NULL);
    }

    for (size_t j = 0; j < COUNT; j++) {
      kfree(blocks[j]);
    }
  }
}

void test_heap() {
  test_heap_alloc();
  test_heap_calloc();
  test_heap_free();
  test_heap_stress();
}
