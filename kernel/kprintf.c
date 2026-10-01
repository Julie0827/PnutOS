#include "kprintf.h"
#include "uart.h"

#include <stdarg.h>
#include <stdint.h>

static void kprint_udec(unsigned long val) {
  if (val >= 10) kprint_udec(val / 10);

  uart_putchar('0' + val % 10);
}

static void kprint_dec(long val) {
  if (val < 0) {
    uart_putchar('-');
    kprint_udec(0UL - (unsigned long)val);

    return;
  }

  kprint_udec(val);
}

static void kprint_hex(unsigned long val) {
  if (val >= 16) kprint_hex(val / 16);

  unsigned int d = val % 16;

  uart_putchar(d < 10 ? '0' + d : 'a' + d - 10);
}

static void kprint_ptr(void *ptr) {
  uart_puts("0x");
  kprint_hex((uintptr_t)ptr);
}

void vkprintf(const char *fmt, va_list args) {
  while (*fmt) {
    if (*fmt != '%') {
      uart_putchar(*fmt++);
      continue;
    }

    if (!*++fmt) {
      uart_putchar('%');
      break;
    }

    switch (*fmt++) {
      case 'c':
        uart_putchar((char)va_arg(args, int));
        break;

      case 's':
        uart_puts(va_arg(args, char *));
        break;

      case 'd':
        kprint_dec(va_arg(args, int));
        break;

      case 'u':
        kprint_udec(va_arg(args, unsigned int));
        break;

      case 'x':
        kprint_hex(va_arg(args, unsigned int));
        break;

      case 'p':
        kprint_ptr(va_arg(args, void *));
        break;

      case '%':
        uart_putchar('%');
        break;

      case 'l':
        if (!*fmt) {
          uart_puts("%l");
          break;
        }

        switch (*fmt++) {
          case 'd':
            kprint_dec(va_arg(args, long));
            break;

          case 'u':
            kprint_udec(va_arg(args, unsigned long));
            break;

          case 'x':
            kprint_hex(va_arg(args, unsigned long));
            break;

          default:
            uart_puts("%l");
            uart_putchar(fmt[-1]);
        }

        break;

      default:
        uart_putchar('%');
        uart_putchar(fmt[-1]);
    }
  }
}

void kprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  vkprintf(fmt, args);

  va_end(args);
}
