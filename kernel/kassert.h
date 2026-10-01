#pragma once

void kassert_fail(const char *condition, const char *file, int line, const char *func);

#define KASSERT(condition)                                                    \
  do {                                                                        \
    if (!(condition)) kassert_fail(#condition, __FILE__, __LINE__, __func__); \
  } while (0)
