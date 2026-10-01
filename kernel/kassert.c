#include "kassert.h"
#include "panic.h"

void kassert_fail(const char *condition, const char *file, int line, const char *func) {
  panic("Assertion failed: %s\r\n  at %s:%d in %s()", condition, file, line, func);
}
