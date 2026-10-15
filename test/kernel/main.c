#include "banner.h"
#include "dtb.h"
#include "interrupt.h"
#include "memory.h"
#include "page.h"
#include "test.h"
#include "test/config.h"
#include "timer.h"
#include "uart.h"

#include <stdint.h>

#define QEMU_TEST_ADDR 0x100000UL
#define QEMU_EXIT_SUCCESS 0x5555

void qemu_exit() {
  *(volatile uint32_t *)QEMU_TEST_ADDR = QEMU_EXIT_SUCCESS;

  while (1) {
  }
}

void kernel_main(unsigned long hart_id, const void *dtb) {
  (void)hart_id;

  uart_init();

  struct dtb_info dtb_info = dtb_parse(dtb);

  memory_init(dtb_info.memory.base, dtb_info.memory.size);
  page_init();

  timer_init(dtb_info.timebase_frequency);

  banner_title("PnutOS Test Suite");

  banner_subtitle("Common");

#if TEST_ALL || TEST_ALIGN
  test_align();
  test_pass("align");
#endif

#if TEST_ALL || TEST_BITMAP
  test_bitmap();
  test_pass("bitmap");
#endif

  banner_subtitle("Libc");

#if TEST_ALL || TEST_STRING
  test_string();
  test_pass("string");
#endif

  banner_subtitle("Kernel");

#if (TEST_ALL || TEST_KPRINTF_MANUAL) && !TEST_DISABLE_MANUAL
  test_manual("kprintf");
  test_kprintf();
#endif

#if TEST_ALL || TEST_PARSER
  test_parser();
  test_pass("parser");
#endif

#if TEST_ALL || TEST_PAGE
  test_page();
  test_pass("page");
#endif

#if TEST_ALL || TEST_HEAP
  test_heap();
  test_pass("heap");
#endif

#if TEST_ALL || TEST_TIMER
  interrupt_enable();
  test_timer();
  interrupt_disable();
  test_pass("timer");
#endif

  banner_summary("All tests passed!");

  qemu_exit();
}
