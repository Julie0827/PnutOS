#include "timer.h"
#include "panic.h"

#define TIMER_INTERVAL_MS 10
#define SIE_STIE (1ULL << 5)

static uint32_t timebase_frequency;
static uint64_t timer_interval_ticks;
static uint64_t start_time;

static uint64_t timer_get_time() {
  uint64_t time;

  __asm__ volatile("rdtime %0" : "=r"(time));

  return time;
}

static void timer_set_compare(uint64_t deadline) {
  __asm__ volatile("csrw stimecmp, %0" : : "r"(deadline));
}

static void timer_enable_interrupt() {
  __asm__ volatile("csrs sie, %0" : : "r"(SIE_STIE));
}

void timer_init(uint32_t frequency) {
  if (!frequency) panic("Invalid timer frequency");

  timebase_frequency = frequency;
  timer_interval_ticks = (uint64_t)frequency * TIMER_INTERVAL_MS / 1000;

  start_time = timer_get_time();

  timer_set_compare(start_time + timer_interval_ticks);
  timer_enable_interrupt();
}

void timer_handle_interrupt() {
  timer_set_compare(timer_get_time() + timer_interval_ticks);
}

uint64_t timer_get_ms() {
  return (timer_get_time() - start_time) * 1000 / timebase_frequency;
}

uint64_t timer_get_s() {
  return timer_get_ms() / 1000;
}
