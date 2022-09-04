#include <stdio.h>

int main() {
  for (int i = 1; i < 18; i++) {
    printf("\e%c\4 ", i);
  }
  printf("\e\x01\n\r");
  for (int i = 0; i < 8; i++) {
    for (int j = 1; j < 18; j++) {
      char c = i * 17 + j;
      printf("%c ", c != 10 && c != 13 ? c : ' ');
    }
    printf("\n\r");
  }
  return 0;
}