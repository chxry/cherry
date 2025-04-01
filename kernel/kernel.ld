ENTRY(_start)

SECTIONS {
  . = 0x80200000;
  _kernel_start = .;

  .text :{
    KEEP(*(.text.start));
    *(.text .text.*);
  }

  _global_pointer = .;
  .rodata : {
    *(.rodata .rodata.*);
  }

  .data : {
    *(.data .data.*);
  }

  .bss : {
    _bss_start = .;
    *(.bss .bss.* .sbss .sbss.*);
    _bss_end = .;
  }

  . = ALIGN(4);
  . += 128 * 1024;
  _stack_top = .;
  
  . = ALIGN(4096);
  _heap_start = .;
}
