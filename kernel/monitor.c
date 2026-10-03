#include "monitor.h"
#include "command.h"
#include "parser.h"
#include "uart.h"

#include <stddef.h>

#define CR '\r'
#define LF '\n'
#define BS '\b'
#define DEL 0x7F

#define LINE_MAX 128

#define ASCII_PRINTABLE_MIN 0x20
#define ASCII_PRINTABLE_MAX 0x7E

static const char prompt[] = "pnut> ";

static void handle_command(char *line) {
  char *argv[ARGV_MAX];
  size_t argc = parse_line(line, argv);

  if (!argc) return;

  if (argc > ARGV_MAX) {
    uart_puts("Too many arguments\r\n");
    return;
  }

  dispatch_command(argc, argv);
}

static void handle_enter(char *line, size_t *len) {
  line[*len] = '\0';

  uart_puts("\r\n");
  handle_command(line);

  *len = 0;

  uart_puts(prompt);
}

static void handle_backspace(size_t *len) {
  if (!*len) return;

  (*len)--;

  uart_puts("\b \b");
}

static void handle_character(char *line, size_t *len, char c) {
  if (*len >= LINE_MAX - 1) return;

  line[(*len)++] = c;

  uart_putchar(c);
}

void monitor_run() {
  char line[LINE_MAX];
  size_t len = 0;

  bool prev_was_cr = false;

  uart_puts(prompt);

  while (1) {
    char c = uart_getchar();

    if (c == LF && prev_was_cr) {
      prev_was_cr = false;
      continue;
    }

    if (c == CR || c == LF) handle_enter(line, &len);
    else if (c == BS || c == DEL) handle_backspace(&len);
    else if (c >= ASCII_PRINTABLE_MIN && c <= ASCII_PRINTABLE_MAX) handle_character(line, &len, c);

    prev_was_cr = (c == CR);
  }
}
