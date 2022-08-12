#pragma once
#include <stdint.h>
#include <limine/limine.h>
#include <tinyalloc/tinyalloc.h>

uint64_t mm_init();

#define mmap mmap_rqeuest.response