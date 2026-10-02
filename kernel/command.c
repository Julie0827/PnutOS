#include "command.h"
#include "cpu.h"
#include "kprintf.h"
#include "memory.h"
#include "string.h"
#include "uart.h"

struct command {
  const char *name;
  const char *description;
  void (*handler)(size_t argc, char **argv);
};

static void cmd_help(size_t argc, char **argv);
static void cmd_about(size_t argc, char **argv);
static void cmd_clear(size_t argc, char **argv);
static void cmd_echo(size_t argc, char **argv);
static void cmd_mem(size_t argc, char **argv);
static void cmd_halt(size_t argc, char **argv);

static const struct command commands[] = {
    {"help", "Show available commands", cmd_help}, {"about", "Show system information", cmd_about},
    {"clear", "Clear the screen", cmd_clear},      {"echo", "Print text", cmd_echo},
    {"mem", "Show memory information", cmd_mem},   {"halt", "Halt the system", cmd_halt},
};

static const size_t command_count = sizeof(commands) / sizeof(commands[0]);

static void cmd_help(size_t argc, char **argv) {
  (void)argc;
  (void)argv;

  uart_puts("Available commands:\r\n");

  for (size_t i = 0; i < command_count; i++) {
    kprintf("%s - %s\r\n", commands[i].name, commands[i].description);
  }
}

static void cmd_about(size_t argc, char **argv) {
  (void)argc;
  (void)argv;

  uart_puts("PnutOS - The Tiny but Mighty OS\r\nArchitecture: RISC-V 64-bit\r\n");
}

static void cmd_clear(size_t argc, char **argv) {
  (void)argc;
  (void)argv;

  uart_puts("\x1b[2J\x1b[H\r\n\r\n");
}

static void cmd_echo(size_t argc, char **argv) {
  for (size_t i = 1; i < argc; i++) {
    if (i > 1) uart_putchar(' ');

    uart_puts(argv[i]);
  }

  uart_puts("\r\n");
}

static void cmd_mem(size_t argc, char **argv) {
  (void)argc;
  (void)argv;

  memory_print_info("");
}

static void cmd_halt(size_t argc, char **argv) {
  (void)argc;
  (void)argv;

  uart_puts("System halted.\r\n");

  cpu_halt();
}

void dispatch_command(size_t argc, char **argv) {
  for (size_t i = 0; i < command_count; i++) {
    if (!strcmp(argv[0], commands[i].name)) {
      commands[i].handler(argc, argv);
      return;
    }
  }

  kprintf("Command not found: %s\r\n", argv[0]);
}
