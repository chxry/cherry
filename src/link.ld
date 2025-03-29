ENTRY(_start)

SECTIONS {
    . = 0x80200000;

    .text : {
        KEEP(*(.text.init));
        *(.text .text.*);
    }

    .rodata : ALIGN(4) {
        *(.rodata .rodata.*);
    }

    .data : ALIGN(4) {
        *(.data .data.*);
    }

    .bss : ALIGN(4) {
        _bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        _bss_end = .;
    }

    . = ALIGN(4);
    . += 128 * 1024;
    _stack_top = .;
}
