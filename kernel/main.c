#include "uart.h"

void kernel_main() {
  uart_init();

  uart_puts("PnutOS\r\n");
  uart_puts("The Tiny but Mighty OS\r\n");

  while (1) {
  }
}
