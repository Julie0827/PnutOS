#include "uart.h"

#define UART_BASE 0x10000000UL

#define UART_THR 0
#define UART_RBR 0
#define UART_DLL 0

#define UART_IER 1
#define UART_DLM 1

#define UART_FCR 2
#define UART_LCR 3
#define UART_LSR 5

#define UART_LCR_DLAB (1U << 7)
#define UART_LCR_8N1 0x03

#define UART_LSR_THRE (1U << 5)
#define UART_LSR_DR 0x01

#define UART_REG(offset) (*(volatile unsigned char *)(UART_BASE + (offset)))

void uart_init() {
  UART_REG(UART_IER) = 0;

  UART_REG(UART_LCR) = UART_LCR_DLAB;

  UART_REG(UART_DLL) = 2;
  UART_REG(UART_DLM) = 0;

  UART_REG(UART_LCR) = UART_LCR_8N1;
}

void uart_putchar(char c) {
  while ((UART_REG(UART_LSR) & UART_LSR_THRE) == 0) {
  }

  UART_REG(UART_THR) = c;
}

void uart_puts(const char *s) {
  while (*s) {
    uart_putchar(*s++);
  }
}

char uart_getchar() {
  while ((UART_REG(UART_LSR) & UART_LSR_DR) == 0) {
  }

  return UART_REG(UART_RBR);
}
