#include "kprintf.h"
#include "string.h"
#include "uart.h"

#include <stddef.h>
#include <stdint.h>

static size_t parse_width(const char **fmt, va_list *args, bool *align_left) {
  size_t width = 0;

  if (**fmt == '*') {
    int dynamic_width = va_arg(*args, int);

    (*fmt)++;

    if (dynamic_width < 0) {
      *align_left = true;
      width = 0U - (unsigned int)dynamic_width;
    } else {
      width = dynamic_width;
    }
  } else {
    while (**fmt >= '0' && **fmt <= '9') {
      width = width * 10 + (*(*fmt)++ - '0');
    }
  }

  return width;
}

static size_t get_pad_len(size_t width, size_t len) {
  return width > len ? width - len : 0;
}

static void kprint_padding(size_t len) {
  for (size_t i = 0; i < len; i++) {
    uart_putchar(' ');
  }
}

static void kprint_char(char c, size_t width, bool align_left) {
  size_t pad_len = get_pad_len(width, 1);

  if (!align_left) kprint_padding(pad_len);

  uart_putchar(c);

  if (align_left) kprint_padding(pad_len);
}

static void kprint_string(const char *s, size_t width, bool align_left) {
  size_t pad_len = get_pad_len(width, strlen(s));

  if (!align_left) kprint_padding(pad_len);

  uart_puts(s);

  if (align_left) kprint_padding(pad_len);
}

static size_t get_num_len(unsigned long val, unsigned int base) {
  size_t len = 1;

  while (val >= base) {
    val /= base;
    len++;
  }

  return len;
}

static void kprint_udec_val(unsigned long val) {
  if (val >= 10) kprint_udec_val(val / 10);

  uart_putchar('0' + val % 10);
}

static void kprint_udec(unsigned long val, size_t width, bool align_left) {
  size_t pad_len = get_pad_len(width, get_num_len(val, 10));

  if (!align_left) kprint_padding(pad_len);

  kprint_udec_val(val);

  if (align_left) kprint_padding(pad_len);
}

static void kprint_dec(long val, size_t width, bool align_left) {
  bool neg = val < 0;
  unsigned long mag = neg ? 0UL - (unsigned long)val : (unsigned long)val;

  size_t pad_len = get_pad_len(width, get_num_len(mag, 10) + neg);

  if (!align_left) kprint_padding(pad_len);

  if (neg) uart_putchar('-');

  kprint_udec_val(mag);

  if (align_left) kprint_padding(pad_len);
}

static void kprint_hex_val(unsigned long val) {
  if (val >= 16) kprint_hex_val(val / 16);

  unsigned int d = val % 16;

  uart_putchar(d < 10 ? '0' + d : 'a' + d - 10);
}

static void kprint_hex(unsigned long val, size_t width, bool align_left) {
  size_t pad_len = get_pad_len(width, get_num_len(val, 16));

  if (!align_left) kprint_padding(pad_len);

  kprint_hex_val(val);

  if (align_left) kprint_padding(pad_len);
}

static void kprint_ptr(void *ptr, size_t width, bool align_left) {
  uintptr_t val = (uintptr_t)ptr;
  size_t pad_len = get_pad_len(width, get_num_len(val, 16) + 2);

  if (!align_left) kprint_padding(pad_len);

  uart_puts("0x");
  kprint_hex_val(val);

  if (align_left) kprint_padding(pad_len);
}

static void kprint_range(const char *start, const char *end) {
  while (start < end) {
    uart_putchar(*start++);
  }
}

void vkprintf(const char *fmt, va_list args) {
  while (*fmt) {
    if (*fmt != '%') {
      uart_putchar(*fmt++);
      continue;
    }

    const char *fmt_start = fmt;

    if (!*++fmt) {
      uart_putchar('%');
      break;
    }

    bool align_left = *fmt == '-';

    if (align_left) fmt++;

    size_t width = parse_width(&fmt, &args, &align_left);

    switch (*fmt++) {
      case 'c':
        kprint_char((char)va_arg(args, int), width, align_left);
        break;

      case 's':
        kprint_string(va_arg(args, char *), width, align_left);
        break;

      case 'd':
        kprint_dec(va_arg(args, int), width, align_left);
        break;

      case 'u':
        kprint_udec(va_arg(args, unsigned int), width, align_left);
        break;

      case 'x':
        kprint_hex(va_arg(args, unsigned int), width, align_left);
        break;

      case 'p':
        kprint_ptr(va_arg(args, void *), width, align_left);
        break;

      case '%':
        if (fmt == fmt_start + 2) uart_putchar('%');
        else kprint_range(fmt_start, fmt);
        break;

      case 'l':
        if (!*fmt) {
          kprint_range(fmt_start, fmt);
          break;
        }

        switch (*fmt++) {
          case 'd':
            kprint_dec(va_arg(args, long), width, align_left);
            break;

          case 'u':
            kprint_udec(va_arg(args, unsigned long), width, align_left);
            break;

          case 'x':
            kprint_hex(va_arg(args, unsigned long), width, align_left);
            break;

          default:
            kprint_range(fmt_start, fmt);
        }

        break;

      default:
        kprint_range(fmt_start, fmt);
    }
  }
}

void kprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  vkprintf(fmt, args);

  va_end(args);
}
