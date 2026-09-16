OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS {
    . = 0x80000000;

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

    .stack ALIGN(16) : {
        stack_bottom = .;
        . += 0x4000;
        stack_top = .;
    }
}
