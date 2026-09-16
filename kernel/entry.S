.section .text
.global _start

_start:
    la sp, stack_top
    call kernel_main

1:
    wfi
    j 1b
