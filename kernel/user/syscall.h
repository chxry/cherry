#pragma once
#include <stdint.h>

#include "sys/mm.h"
#include "boot/idt.h"

#define RETURN(x) asm volatile("mov %0,%%rax" ::"a"(x))

void __attribute__((interrupt)) syscall_handler(frame_t* frame);
