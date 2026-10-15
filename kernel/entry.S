.section .text
.global _start
.extern supervisor_trap_entry

_start:
    la sp, stack_top

    call set_mpp_supervisor
    call setup_pmp
    call setup_address_translation
    call setup_traps
    call setup_timer_access

    la t0, supervisor_entry
    csrw mepc, t0

    mret

set_mpp_supervisor:
    csrr t0, mstatus

    li t1, ~(3 << 11)
    and t0, t0, t1

    li t1, (1 << 11)
    or t0, t0, t1

    csrw mstatus, t0

    ret

setup_pmp:
    li t0, -1
    csrw pmpaddr0, t0

    li t0, 0x1F
    csrw pmpcfg0, t0

    ret

setup_address_translation:
    csrw satp, zero

    ret

setup_traps:
    la t0, machine_trap_entry
    csrw mtvec, t0

    li t0, 0xB1FF
    csrw medeleg, t0

    li t0, 0x222
    csrw mideleg, t0

    ret

setup_timer_access:
    csrr t0, mcounteren

    li t1, (1 << 1)
    or t0, t0, t1

    csrw mcounteren, t0

    csrr t0, menvcfg

    li t1, (1 << 63)
    or t0, t0, t1

    csrw menvcfg, t0

    ret

supervisor_entry:
    csrw sscratch, zero

    la t0, supervisor_trap_entry
    csrw stvec, t0

    call kernel_main

1:
    wfi
    j 1b

.align 2
machine_trap_entry:
    # TODO: Handle traps in M-mode

1:
    j 1b
