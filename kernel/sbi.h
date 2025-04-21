#pragma once
#include <stddef.h>

typedef struct {
  size_t error;
  size_t value;
} sbiret_t;

sbiret_t sbi_call(size_t arg0, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t fid, size_t eid);
