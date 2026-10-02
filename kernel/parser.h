#pragma once

#include <stddef.h>

#define ARGV_MAX 16

size_t parse_line(char *line, char **argv);
