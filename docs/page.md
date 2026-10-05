# Physical Page Allocator

The physical page allocator manages allocatable physical pages.

## Configuration

- Page size: `4096` bytes

## Memory Layout

- `bitmap_addr` - first page-aligned address after the kernel stack
- `first_page_addr` - address of the first allocatable physical page
- `last_page_addr` - address of the last allocatable physical page

## Bitmap

The bitmap tracks whether each physical page is free or allocated. Each physical page is represented by one bit in the bitmap.

- `0` - allocated
- `1` - free

### Bitmap Page Count

To determine the bitmap page count, let:

- `N` - total number of pages available for the bitmap and allocatable pages
- `x` - bitmap page count

The number of allocatable pages is `N - x`. Since each bitmap page can store `PAGE_SIZE * 8` bits, the following must hold:

```text
x * PAGE_SIZE * 8 >= N - x
```

Rearranging:

```text
x * (PAGE_SIZE * 8 + 1) >= N
```

Therefore:

```text
x = ceil(N / (PAGE_SIZE * 8 + 1))
```

## Free List

Free pages are maintained as a singly linked list. Each free page stores a pointer to the next free page at the beginning of the page.

## Allocation

Pages are allocated from the free list and zeroed before use.

## Freeing

Freed pages are returned to the free list. Invalid frees are detected, and the bitmap is used to detect double frees.

## Software Interface

```c
void page_init();
void *page_alloc();
void page_free(void *ptr);
```

- `page_init()` - initializes the physical page allocator
- `page_alloc()` - allocates a physical page
- `page_free()` - frees a physical page
