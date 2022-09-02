#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "io/io.h"
#include "lib/string.h"
#include "apps/term.h"

void __attribute__((interrupt)) kb_handler(void* frame);