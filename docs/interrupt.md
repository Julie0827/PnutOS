# Interrupts

PnutOS provides global S-mode interrupt control using the `SIE` (Supervisor Interrupt Enable) bit in the `sstatus` CSR.

## Interrupt State

- `SIE = 0` - interrupts are disabled
- `SIE = 1` - interrupts are enabled

## Software Interface

```c
void interrupt_enable();
bool interrupt_disable();
void interrupt_restore(bool was_enabled);
```

- `interrupt_enable()` - enables interrupts
- `interrupt_disable()` - disables interrupts and returns the previous interrupt state
- `interrupt_restore()` - restores the previous interrupt state
