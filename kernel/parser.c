#include "parser.h"

static bool is_whitespace(char c) {
  return c == ' ' || c == '\t';
}

size_t parse_line(char *line, char **argv) {
  size_t argc = 0;
  char *ptr = line;

  bool prev_was_whitespace = true;

  while (*ptr) {
    bool whitespace = is_whitespace(*ptr);

    if (!whitespace && prev_was_whitespace && argc < ARGV_MAX) argv[argc++] = ptr;
    else if (whitespace) *ptr = '\0';

    prev_was_whitespace = whitespace;
    ptr++;
  }

  return argc;
}
