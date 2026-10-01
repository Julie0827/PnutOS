OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS {
    . = 0x80000000;

    kernel_start = .;

    .text : {
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    }

    kernel_end = .;

    .stack ALIGN(16) : {
        stack_bottom = .;
        . += 0x4000;
        stack_top = .;
    }
}
