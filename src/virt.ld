OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY {
	ram (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS {
	.text : ALIGN(4K) {
	  *(.init);
    *(.text);
  } > ram

  .bss : ALIGN(4K) {
	  PROVIDE(bss_start = .);
    *(.bss);
    . += 4096;
    PROVIDE(stack_top = .);
    . += 4096;
    PROVIDE(global_pointer = .);
    PROVIDE(bss_end = .);
  } > ram

	.rodata : ALIGN(4K) {
	  *(.rodata);
  } > ram

  .data : ALIGN(4K) {
    *(.data);
  } > ram
}
