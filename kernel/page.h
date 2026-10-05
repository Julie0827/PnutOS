#pragma once

#define PAGE_SIZE 4096

void page_init();
void *page_alloc();
void page_free(void *ptr);
