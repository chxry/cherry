#pragma once
#include <stdint.h>

#include "io/io.h"

#define PIT_FREQ 1193181

extern uint64_t ticks;

void pit_init(uint16_t hz);
void __attribute__((interrupt)) pit_handler(void* frame);
void sleep(uint64_t ms);
