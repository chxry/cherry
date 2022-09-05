#include <stdio.h>
#include <time.h>

int main() {
  time_t now = time();
  printf("%02i:%02i:%02i %02i/%02i/%i\n\r", now.hour, now.minute, now.second, now.day, now.month, now.year);
  return 0;
}