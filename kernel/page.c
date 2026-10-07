#include "page.h"
#include "align.h"
#include "bitmap.h"
#include "kassert.h"
#include "memory.h"
#include "string.h"

#include <stddef.h>
#include <stdint.h>

extern char stack_top[];

struct free_page {
  struct free_page *next;
};

static struct free_page *free_list;

static uintptr_t bitmap_addr;
static uint8_t *bitmap;

static uintptr_t first_page_addr;
static uintptr_t last_page_addr;

static size_t page_bitmap_page_count(size_t total_page_count) {
  size_t divisor = PAGE_SIZE * 8 + 1;

  return total_page_count / divisor + (total_page_count % divisor != 0);
}

static size_t page_index(void *ptr) {
  return ((uintptr_t)ptr - first_page_addr) / PAGE_SIZE;
}

void page_init() {
  bitmap_addr = align_up_addr((uintptr_t)stack_top, PAGE_SIZE);
  bitmap = (uint8_t *)bitmap_addr;

  last_page_addr = align_down_addr((uintptr_t)memory_get_end(), PAGE_SIZE) - PAGE_SIZE;

  size_t total_page_count = (last_page_addr - bitmap_addr) / PAGE_SIZE + 1;
  size_t bitmap_page_count = page_bitmap_page_count(total_page_count);

  first_page_addr = bitmap_addr + PAGE_SIZE * bitmap_page_count;

  memset(bitmap, 0, bitmap_page_count * PAGE_SIZE);

  for (uintptr_t addr = last_page_addr; addr >= first_page_addr; addr -= PAGE_SIZE) {
    struct free_page *page = (struct free_page *)addr;

    page->next = free_list;
    free_list = page;

    bitmap_set(bitmap, page_index(page));
  }
}

void *page_alloc() {
  if (!free_list) return NULL;

  struct free_page *page = free_list;

  free_list = page->next;

  bitmap_clear(bitmap, page_index(page));
  memset(page, 0, PAGE_SIZE);

  return page;
}

static bool page_is_valid(void *ptr) {
  uintptr_t addr = (uintptr_t)ptr;

  if (addr < first_page_addr || addr > last_page_addr) return false;

  if (addr & (PAGE_SIZE - 1)) return false;

  return true;
}

static bool page_is_free(void *ptr) {
  return bitmap_test(bitmap, page_index(ptr));
}

void page_free(void *ptr) {
  KASSERT(page_is_valid(ptr));
  KASSERT(!page_is_free(ptr));

  struct free_page *page = (struct free_page *)ptr;

  page->next = free_list;
  free_list = page;

  bitmap_set(bitmap, page_index(page));
}
