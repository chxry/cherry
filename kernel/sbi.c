#include "sbi.h"

sbiret_t sbi_call(size_t arg0, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t fid, size_t eid) {
  register size_t a0 asm("a0") = arg0;
  register size_t a1 asm("a1") = arg1;
  register size_t a2 asm("a2") = arg2;
  register size_t a3 asm("a3") = arg3;
  register size_t a4 asm("a4") = arg4;
  register size_t a5 asm("a5") = arg5;
  register size_t a6 asm("a6") = fid;
  register size_t a7 asm("a7") = eid;

  asm volatile("ecall"
    : "=r"(a0), "=r"(a1)
    : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
    : "memory"
  );
  return (sbiret_t){ .error = a0, .value = a1 };
}
