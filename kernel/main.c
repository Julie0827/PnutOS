#include "diagnostics.h"
#include "dtb.h"
#include "memory.h"
#include "monitor.h"
#include "page.h"
#include "uart.h"

void kernel_main(unsigned long hart_id, const void *dtb) {
  uart_init();

  uart_puts("PnutOS\r\n");
  uart_puts("The Tiny but Mighty OS\r\n");

  struct dtb_memory memory = dtb_get_memory(dtb);

  memory_init(memory.base, memory.size);

  diagnostics_print(hart_id);

  page_init();

  monitor_run();

  while (1) {
  }
}
