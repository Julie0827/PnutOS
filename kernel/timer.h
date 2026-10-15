#pragma once

#include <stdint.h>

void timer_init(uint32_t frequency);
void timer_handle_interrupt();

uint64_t timer_get_ms();
uint64_t timer_get_s();
