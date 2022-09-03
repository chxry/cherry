#include "prog.h"

int prog_exec(char* path) {
  return ((prog_t)(fs_read(fs_search_path(path))))();
}
