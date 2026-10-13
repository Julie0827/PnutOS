#include "trap.h"
#include "ansi.h"
#include "banner.h"
#include "kprintf.h"
#include "panic.h"

enum exception_code {
  EXCEPTION_INST_ADDR_MISALIGNED = 0,
  EXCEPTION_INST_ACCESS_FAULT = 1,
  EXCEPTION_ILLEGAL_INSTRUCTION = 2,
  EXCEPTION_BREAKPOINT = 3,
  EXCEPTION_LOAD_ADDR_MISALIGNED = 4,
  EXCEPTION_LOAD_ACCESS_FAULT = 5,
  EXCEPTION_STORE_AMO_ADDR_MISALIGNED = 6,
  EXCEPTION_STORE_AMO_ACCESS_FAULT = 7,
  EXCEPTION_ECALL_U = 8,
  EXCEPTION_INST_PAGE_FAULT = 12,
  EXCEPTION_LOAD_PAGE_FAULT = 13,
  EXCEPTION_STORE_AMO_PAGE_FAULT = 15,
};

// clang-format off
#define GPR_LIST(X) \
  X(ra) X(sp) X(gp) X(tp) \
  X(t0) X(t1) X(t2) X(t3) X(t4) X(t5) X(t6) \
  X(s0) X(s1) X(s2) X(s3) X(s4) X(s5) X(s6) X(s7) X(s8) X(s9) X(s10) X(s11) \
  X(a0) X(a1) X(a2) X(a3) X(a4) X(a5) X(a6) X(a7)
// clang-format on

#define CSR_LIST(X) X(sstatus) X(sepc) X(scause) X(stval)

#define PRINT_REG(reg) kprintf("%-8s 0x%lx\r\n", #reg, frame->reg);

static void print_trap_frame(const struct trap_frame *frame) {
  banner_subtitle("GPRs");

  GPR_LIST(PRINT_REG)

  banner_subtitle("CSRs");

  CSR_LIST(PRINT_REG)
}

#undef GPR_LIST
#undef CSR_LIST
#undef PRINT_REG

static void handle_interrupt(struct trap_frame *frame, uint64_t code) {}

static void handle_exception(struct trap_frame *frame, uint64_t code) {
  const char *description;

  switch (code) {
    case EXCEPTION_INST_ADDR_MISALIGNED:
      description = "Instruction address misaligned";
      break;

    case EXCEPTION_INST_ACCESS_FAULT:
      description = "Instruction access fault";
      break;

    case EXCEPTION_ILLEGAL_INSTRUCTION:
      description = "Illegal instruction";
      break;

    case EXCEPTION_BREAKPOINT:
      description = "Breakpoint";
      break;

    case EXCEPTION_LOAD_ADDR_MISALIGNED:
      description = "Load address misaligned";
      break;

    case EXCEPTION_LOAD_ACCESS_FAULT:
      description = "Load access fault";
      break;

    case EXCEPTION_STORE_AMO_ADDR_MISALIGNED:
      description = "Store/AMO address misaligned";
      break;

    case EXCEPTION_STORE_AMO_ACCESS_FAULT:
      description = "Store/AMO access fault";
      break;

    case EXCEPTION_ECALL_U:
      description = "Environment call from U-mode";
      break;

    case EXCEPTION_INST_PAGE_FAULT:
      description = "Instruction page fault";
      break;

    case EXCEPTION_LOAD_PAGE_FAULT:
      description = "Load page fault";
      break;

    case EXCEPTION_STORE_AMO_PAGE_FAULT:
      description = "Store/AMO page fault";
      break;

    default:
      description = "Unknown exception";
  }

  kprintf(ANSI_RED "Exception: %s" ANSI_RESET "\r\n", description);
  print_trap_frame(frame);
}

void trap_handler(struct trap_frame *frame) {
  uint64_t scause = frame->scause;

  bool is_interrupt = scause >> 63;
  uint64_t code = scause & (UINT64_MAX >> 1);

  if (is_interrupt) handle_interrupt(frame, code);
  else handle_exception(frame, code);

  panic("Unhandled trap (scause=0x%lx)", scause);
}
