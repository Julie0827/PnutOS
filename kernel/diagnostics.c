#include "diagnostics.h"
#include "config.h"
#include "kprintf.h"
#include "memory.h"

static const char boot_prefix[] = "[boot] ";
static const char diag_prefix[] = "[diag] ";

void diagnostics_print(unsigned long hart_id) {
#if CONFIG_DIAGNOSTICS
  kprintf("%shart: %lu\n", boot_prefix, hart_id);

  memory_print_info(diag_prefix);
#endif
}
