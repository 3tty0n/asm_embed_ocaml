#include <caml/compatibility.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

enum jit_type {TJ, MJ};

int counter = 0;

char *gen_trace_name(enum jit_type typ) {
  char *str;
  if (typ == TJ) {
	sprintf(str, "tracetj%d", counter);
  } else if (typ == MJ) {
	sprintf(str, "tracemj%d", counter);
  } else {
	printf(stderr, "invalid jit_type");
	exit(EXIT_FAILURE);
  }
  counter += 1;
  assert(str[41] == '\0');
  return str;
}

void count_up(int pc) {
  return;
}
