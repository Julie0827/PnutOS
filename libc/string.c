#include "string.h"

size_t strlen(const char *s) {
  size_t len = 0;

  while (*s++) len++;

  return len;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 && *s1 == *s2) {
    s1++;
    s2++;
  }

  return (unsigned char)*s1 - (unsigned char)*s2;
}

void *memset(void *ptr, int val, size_t count) {
  unsigned char *bytes = ptr;

  for (size_t i = 0; i < count; i++) {
    bytes[i] = val;
  }

  return ptr;
}
