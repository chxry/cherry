#pragma once
#include <stdint.h>
#include <tinyalloc/tinyalloc.h>

#include "sys/interrupts.h"

#define RETURN(x) asm volatile("mov %0,%%rax" ::"a"(x))

void __attribute__((interrupt)) syscall_handler(frame_t* frame);
