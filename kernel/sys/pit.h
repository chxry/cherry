#pragma once
#include <stdint.h>

#include "io/io.h"
#include "io/serial.h"

#define PIT_FREQ 1193181

extern uint64_t ticks;

void pit_init(uint16_t hz);
uint64_t get_pit_ticks();
void sleep(uint64_t ms);
