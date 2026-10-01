#include "diagnostics.h"
#include "dtb.h"
#include "uart.h"

void kernel_main(unsigned long hart_id, const void *dtb) {
  uart_init();

  uart_puts("PnutOS\r\n");
  uart_puts("The Tiny but Mighty OS\r\n");

  diagnostics_print(hart_id, dtb_get_memory(dtb));

  while (1) {
  }
}
