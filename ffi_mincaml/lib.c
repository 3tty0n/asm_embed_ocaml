#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern unsigned long
get_current_micros(void) asm("min_caml_get_current_micros");

extern void
call_test_dummy(int *, int, int *, int) asm ("call_test_dummy");

unsigned long get_current_micros() {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  return current_time.tv_sec * (int)1e6 + current_time.tv_usec;
}

void call_test_dummy(int *stk, int sp, int *code, int pc) {
  printf("test_dummy is called.\n");
  return;
}
