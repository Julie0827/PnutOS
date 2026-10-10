#include "kassert.h"
#include "parser.h"
#include "string.h"
#include "test.h"

#include <stddef.h>

static void assert_parsed_echo(char *line) {
  char *argv[ARGV_MAX];

  size_t argc = parse_line(line, argv);

  KASSERT(argc == 3);
  KASSERT(strcmp(argv[0], "echo") == 0);
  KASSERT(strcmp(argv[1], "hello") == 0);
  KASSERT(strcmp(argv[2], "world") == 0);
}

static void test_parser_basic() {
  char line[] = "echo hello world";

  assert_parsed_echo(line);
}

static void test_parser_whitespace() {
  char line[] = "   echo    hello\tworld   ";

  assert_parsed_echo(line);
}

static void test_parser_empty() {
  char empty[] = "";
  char spaces[] = "   \t   ";

  char *argv[ARGV_MAX];

  KASSERT(parse_line(empty, argv) == 0);
  KASSERT(parse_line(spaces, argv) == 0);
}

static void test_parser_too_many_args() {
  char line[4 * ARGV_MAX];
  size_t len = sizeof(line);

  char *argv[ARGV_MAX];

  for (size_t i = 0; i < len; i += 2) {
    line[i] = 'a';
    line[i + 1] = ' ';
  }

  line[len - 1] = '\0';

  KASSERT(parse_line(line, argv) == ARGV_MAX + 1);
}

void test_parser() {
  test_parser_basic();
  test_parser_whitespace();
  test_parser_empty();
  test_parser_too_many_args();
}
