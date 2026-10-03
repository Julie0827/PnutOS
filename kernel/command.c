#include "command.h"
#include "ansi.h"
#include "cpu.h"
#include "kprintf.h"
#include "memory.h"
#include "parser.h"
#include "string.h"
#include "uart.h"

struct command {
  const char *name;
  const char *description;
  const char *usage;
  size_t min_argc;
  size_t max_argc;
  void (*handler)(size_t argc, char **argv);
};

static void cmd_help(size_t argc, char **argv);
static void cmd_about(size_t argc, char **argv);
static void cmd_clear(size_t argc, char **argv);
static void cmd_echo(size_t argc, char **argv);
static void cmd_mem(size_t argc, char **argv);
static void cmd_halt(size_t argc, char **argv);

static const struct command commands[] = {
    {"help", "Show available commands", "help [command]", 1, 2, cmd_help},
    {"about", "Show system information", "about", 1, 1, cmd_about},
    {"clear", "Clear the screen", "clear", 1, 1, cmd_clear},
    {"echo", "Print text", "echo [text ...]", 1, ARGV_MAX, cmd_echo},
    {"mem", "Show memory information", "mem", 1, 1, cmd_mem},
    {"halt", "Halt the system", "halt", 1, 1, cmd_halt},
};

static const size_t command_count = sizeof(commands) / sizeof(commands[0]);

static void print_command(const struct command *command, bool align) {
  const char *format = align ? "%-10s%s\r\n" : "%s - %s\r\n";

  kprintf(format, command->name, command->description);
}

static void print_usage(const struct command *command) {
  kprintf("Usage: %s\r\n", command->usage);
}

static void print_command_not_found(const char *name) {
  kprintf("Command not found: %s\r\n", name);
}

static void cmd_help(size_t argc, char **argv) {
  if (argc == 1) {
    uart_puts("Available commands:\r\n");

    for (size_t i = 0; i < command_count; i++) {
      print_command(&commands[i], true);
    }

    return;
  }

  for (size_t i = 0; i < command_count; i++) {
    if (!strcmp(argv[1], commands[i].name)) {
      print_command(&commands[i], false);
      print_usage(&commands[i]);

      return;
    }
  }

  print_command_not_found(argv[1]);
}

static void cmd_about(size_t argc, char **argv) {
  (void)argc;
  (void)argv;

  uart_puts("PnutOS - The Tiny but Mighty OS\r\nArchitecture: RISC-V 64-bit\r\n");
}

static void cmd_clear(size_t argc, char **argv) {
  (void)argc;
  (void)argv;

  uart_puts(ANSI_CLEAR_SCREEN ANSI_CURSOR_HOME "\r\n\r\n");
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
      if (argc < commands[i].min_argc || argc > commands[i].max_argc) {
        print_usage(&commands[i]);
        return;
      }

      commands[i].handler(argc, argv);
      return;
    }
  }

  print_command_not_found(argv[0]);
}
