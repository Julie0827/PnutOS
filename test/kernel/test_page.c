#include "kassert.h"
#include "page.h"
#include "string.h"
#include "test.h"

#include <stddef.h>
#include <stdint.h>

static void test_page_alloc() {
  enum { COUNT = 10 };

  void *pages[COUNT];

  for (size_t i = 0; i < COUNT; i++) {
    pages[i] = page_alloc();

    KASSERT(pages[i] != NULL);
    KASSERT((uintptr_t)pages[i] % PAGE_SIZE == 0);

    for (size_t j = 0; j < i; j++) {
      KASSERT(pages[i] != pages[j]);
    }
  }

  for (size_t i = 0; i < COUNT; i++) {
    page_free(pages[i]);
  }
}

static void test_page_reuse() {
  void *page1 = page_alloc();
  void *page2 = page_alloc();

  KASSERT(page1 != NULL);
  KASSERT(page2 != NULL);
  KASSERT(page1 != page2);

  page_free(page1);
  page_free(page2);

  KASSERT(page_alloc() == page2);
  KASSERT(page_alloc() == page1);

  page_free(page1);
  page_free(page2);
}

static void test_page_zeroed() {
  uint8_t *page = page_alloc();

  KASSERT(page != NULL);

  memset(page, 0xAB, PAGE_SIZE);
  page_free(page);

  uint8_t *reused = page_alloc();

  KASSERT(reused == page);

  for (size_t i = 0; i < PAGE_SIZE; i++) {
    KASSERT(reused[i] == 0);
  }

  page_free(reused);
}

void test_page() {
  test_page_alloc();
  test_page_reuse();
  test_page_zeroed();
}
