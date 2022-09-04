#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "boot/idt.h"
#include "io/io.h"
#include "apps/term.h"
#include "string.h"

void __attribute__((interrupt)) kb_handler(frame_t* frame);