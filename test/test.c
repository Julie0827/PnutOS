#include "test.h"
#include "kprintf.h"
#include "uart.h"

#define PASS_COLOR ANSI_GREEN
#define MANUAL_COLOR ANSI_YELLOW

void test_pass(const char *name) {
  kprintf(PASS_COLOR "[PASS]" ANSI_RESET " %s\r\n", name);
}

void test_manual(const char *name) {
  kprintf(MANUAL_COLOR "[MANUAL]" ANSI_RESET " %s\r\n", name);
}

void test_blank_line() {
  uart_puts("\r\n");
}
