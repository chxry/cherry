#pragma once
#include <stdint.h>

#include "boot/idt.h"
#include "sys/mm.h"
#include "sys/time.h"

#define RETURN(x) asm volatile("mov %0,%%rax" ::"a"(x))

void __attribute__((interrupt)) syscall_handler(frame_t* frame);
