#pragma once

#include "ansi.h"

#define TEST_EXPECTED ANSI_GREEN "  Expected: " ANSI_RESET
#define TEST_ACTUAL ANSI_YELLOW "    Actual: " ANSI_RESET

void test_pass(const char *name);
void test_manual(const char *name);
void test_blank_line();

// common
void test_align();
void test_bitmap();

// libc
void test_string();

// kernel
void test_kprintf();
void test_parser();
void test_page();
void test_heap();
