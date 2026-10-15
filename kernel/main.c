#include "diagnostics.h"
#include "dtb.h"
#include "interrupt.h"
#include "memory.h"
#include "monitor.h"
#include "page.h"
#include "timer.h"
#include "uart.h"

void kernel_main(unsigned long hart_id, const void *dtb) {
  uart_init();

  uart_puts("PnutOS\r\n");
  uart_puts("The Tiny but Mighty OS\r\n");

  struct dtb_info dtb_info = dtb_parse(dtb);

  memory_init(dtb_info.memory.base, dtb_info.memory.size);
  page_init();

  timer_init(dtb_info.timebase_frequency);

  diagnostics_print(hart_id);

  interrupt_enable();

  monitor_run();

  while (1) {
  }
}
