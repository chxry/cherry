#pragma once

#include "fs/fs.h"

typedef int (*prog_t)(void);

int prog_exec(char* path);
