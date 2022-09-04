#include "string.h"

bool strcmp(char* a, char* b) {
  while (*a) {
    if (*a++ != *b++) {
      return false;
    }
  }
  return !*b;
}

uint64_t strlen(char* str) {
  uint64_t n = 0;
  for (; str[n]; n++)
    ;
  return n;
}

uint64_t strfind(char* str, char chr) {
  uint64_t n = 0;
  for (; str[n] != chr; n++) {
    if (!str[n]) {
      return 0;
    }
  }
  return n;
}