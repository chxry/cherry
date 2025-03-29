.section .text.init
.global _start

_start:
  # wfi
  # .option push
  # .option norelax
  # la gp, global_pointer
  # .option pop

  # csrr t0, mhartid
  # bnez t0, 1f # wait if we arent hart 0
  
  la sp, _stack_top
  # la t0, kmain
  # csrw mepc, t0
  tail kmain

1:
  wfi
  j 1b
