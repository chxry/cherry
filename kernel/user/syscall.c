#include "syscall.h"

void __attribute__((interrupt)) syscall_handler(frame_t* frame) {
  uint64_t* stack = (uint64_t*)frame->rsp;
  switch (stack[0]) {
  case 1: // print
    term_print((char*)stack[1]);
    break;
  case 2: // malloc
    RETURN(kmalloc(stack[1]));
    break;
  case 3: // free
    kfree((void*)stack[1]);
    break;
  case 4: { // time
    time_t time = get_time();
    memcpy((void*)stack[1], &time, sizeof(time_t));
    break;
  }
  default:
    logf("Unknown syscall %lu", stack[0]);
  }
}