#include "heap.h"
#include "align.h"
#include "kassert.h"
#include "page.h"
#include "string.h"

#define HEAP_ALIGNMENT alignof(max_align_t)

struct heap_block {
  size_t size;
  struct heap_block *next;
};

static const size_t metadata_size = sizeof(struct heap_block);
static const size_t min_block_size = metadata_size + HEAP_ALIGNMENT;

static struct heap_block *free_list;

static struct heap_block *heap_split_block(struct heap_block *block, size_t size) {
  struct heap_block *remaining = (struct heap_block *)((uint8_t *)block + metadata_size + size);

  remaining->size = block->size - size - metadata_size;
  remaining->next = block->next;

  block->size = size;

  return remaining;
}

static void heap_update_link(struct heap_block *prev, struct heap_block *block) {
  if (prev) prev->next = block;
  else free_list = block;
}

static void *heap_alloc_block(struct heap_block *block, struct heap_block *prev, size_t size) {
  if (block->size - size >= min_block_size) heap_update_link(prev, heap_split_block(block, size));
  else heap_update_link(prev, block->next);

  return block + 1;
}

static void heap_validate_insert(struct heap_block *before, struct heap_block *block,
                                 struct heap_block *after) {
  KASSERT(!before || (uintptr_t)before + metadata_size + before->size <= (uintptr_t)block);
  KASSERT(!after || (uintptr_t)block + metadata_size + block->size <= (uintptr_t)after);
}

static void heap_merge_blocks(struct heap_block *b1, struct heap_block *b2) {
  if ((uintptr_t)b1 + metadata_size + b1->size != (uintptr_t)b2) return;

  b1->size += metadata_size + b2->size;
  b1->next = b2->next;
}

static void heap_insert_block(struct heap_block *block) {
  struct heap_block *curr = free_list;
  struct heap_block *prev = NULL;

  while (curr && (uintptr_t)curr < (uintptr_t)block) {
    prev = curr;
    curr = curr->next;
  }

  heap_validate_insert(prev, block, curr);

  block->next = curr;

  heap_update_link(prev, block);

  if (curr) heap_merge_blocks(block, curr);
  if (prev) heap_merge_blocks(prev, block);
}

static bool heap_grow() {
  struct heap_block *block = page_alloc();

  if (!block) return false;

  block->size = PAGE_SIZE - metadata_size;

  heap_insert_block(block);

  return true;
}

void *kmalloc(size_t size) {
  if (!size) return NULL;
  if (align_up_size_overflows(size, HEAP_ALIGNMENT)) return NULL;

  size = align_up_size(size, HEAP_ALIGNMENT);

  while (1) {
    struct heap_block *block = free_list;
    struct heap_block *prev = NULL;

    while (block) {
      if (block->size >= size) return heap_alloc_block(block, prev, size);

      prev = block;
      block = block->next;
    }

    if (!heap_grow()) return NULL;
  }
}

void *kcalloc(size_t count, size_t size) {
  if (!count || !size) return NULL;
  if (size > SIZE_MAX / count) return NULL;

  size_t total = count * size;

  void *ptr = kmalloc(total);

  if (!ptr) return NULL;

  return memset(ptr, 0, total);
}

void kfree(void *ptr) {
  if (!ptr) return;

  heap_insert_block((struct heap_block *)ptr - 1);
}
