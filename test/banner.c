#include "banner.h"
#include "ansi.h"
#include "kassert.h"
#include "kprintf.h"
#include "string.h"
#include "uart.h"

#include <stddef.h>

#define BANNER_WIDTH 60

#define TITLE_COLOR ANSI_CYAN
#define SUBTITLE_COLOR ANSI_BLUE
#define SUMMARY_COLOR ANSI_GREEN

static void repeat_char(char c, size_t count) {
  for (size_t i = 0; i < count; i++) {
    uart_putchar(c);
  }
}

static void banner_border() {
  uart_putchar('+');
  repeat_char('-', BANNER_WIDTH - 2);
  uart_puts("+\r\n");
}

static void banner_box(const char *text, const char *color) {
  size_t len = strlen(text);

  KASSERT(len <= BANNER_WIDTH - 2);

  size_t remaining = BANNER_WIDTH - len;
  size_t left = remaining / 2;
  size_t right = remaining - left;

  uart_puts(color);
  banner_border();
  kprintf("%*c%s%*c\r\n", -(int)left, '|', text, (int)right, '|');
  banner_border();
  uart_puts(ANSI_RESET);
}

void banner_title(const char *text) {
  banner_box(text, TITLE_COLOR);
}

void banner_subtitle(const char *text) {
  size_t len = strlen(text);

  KASSERT(len <= BANNER_WIDTH - 4);

  size_t padding = BANNER_WIDTH - len - 4;
  size_t left = padding / 2;
  size_t right = padding - left;

  uart_puts(SUBTITLE_COLOR);
  repeat_char('=', left);
  kprintf("[ %s ]", text);
  repeat_char('=', right);
  uart_puts(ANSI_RESET "\r\n");
}

void banner_summary(const char *text) {
  banner_box(text, SUMMARY_COLOR);
}
