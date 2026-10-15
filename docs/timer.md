# Timer

PnutOS uses the RISC-V hardware timer for periodic interrupts and timekeeping. The timer frequency is discovered from the DTB during boot.

## Configuration

- Timer interrupt interval: `10 ms`

## Timer Interrupts

The hardware `time` counter continuously tracks time. Each time the counter reaches the deadline set in the `stimecmp` CSR, an interrupt occurs and the next deadline is set.

## Timekeeping

Elapsed time is tracked since timer initialization using the hardware `time` counter. Timekeeping is independent of timer interrupts.

## Software Interface

```c
void timer_init(uint32_t frequency);
void timer_handle_interrupt();
uint64_t timer_get_ms();
uint64_t timer_get_s();
```

- `timer_init()` - initializes the timer
- `timer_handle_interrupt()` - handles a timer interrupt and sets the next deadline
- `timer_get_ms()` - returns elapsed time in milliseconds
- `timer_get_s()` - returns elapsed time in seconds
