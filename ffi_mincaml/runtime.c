#define _GNU_SOURCE

#include <assert.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/compatibility.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <dlfcn.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "modwrap.h"

#define PROF_LEN 100
#define THOLD 100

#define JIT_COMPILE_COMMAND "gcc -m32 -fPIC"

typedef int (*fun_arg2)(int, int);

enum jit_type { TJ, MJ };

/**
 * For gen_trace_name
 */
int counter_name = 0;

/**
 * Generate the name of a trace
 * - tracing JIT: tracetj0, tracetj1, ...
 * - method JIT: tracemj0, tracemj1, ...
 */
void gen_trace_name(char *buffer, enum jit_type typ) {
  if (typ == TJ) {
    sprintf(buffer, "tracetj%d", counter_name);
  } else if (typ == MJ) {
    sprintf(buffer, "tracemj%d", counter_name);
  } else {
    fprintf(stderr, "invalid jit_type");
    exit(EXIT_FAILURE);
  }
  counter_name += 1;
  return;
}

void gen_so_name(char *buffer, char *tname) {
  sprintf(buffer, "./lib%s.so", tname);
}

int prof_arr[2048] = {0};

fun_arg2 sym_arr[2048] = {NULL};

fun_arg2 call_dlfun_arg2(char *filename, char *funcname, int pc, int arg1,
                         int arg2) {
  fun_arg2 sym = NULL;
  void *handle = NULL;
  int res;

  handle = dlopen(filename, RTLD_LAZY | RTLD_GLOBAL);
  if (handle == NULL) {
    fprintf(stderr, "error: dlopen %s\n", filename);
    return NULL;
  }
  dlerror();

  sym = (fun_arg2)dlsym(RTLD_DEFAULT, funcname);
  if (sym == NULL) {
    fprintf(stderr, "error: dlsym \n");
    return NULL;
  }
  sym_arr[pc] = sym;
  printf("added sym:\t%s\t%d\n", filename, pc);
  return sym;
}

void jit_compile(char *so, char *func, int pc) {
  char buffer[1024];

  printf("compiling shared object: %s\n", so);
  sprintf(buffer, "%s -c %s.s", JIT_COMPILE_COMMAND, func);
  system(buffer);

  sprintf(buffer, "%s -shared -rdynamic -o %s %s.o", JIT_COMPILE_COMMAND, so,
          func);
  system(buffer);

  return;
}

void call_jit_merge_point(int *stack, int sp, int *code, int pc) {
  char tname[136];
  char so[136];
  char func[10];
  struct prof *p;
  fun_arg2 sym = NULL;
  int res;

  printf("pc: %d\n", pc);

  if (pc == 0 || pc == 4)
    strcpy(func, "add");
  else if (pc == 2)
    strcpy(func, "sub");

  sym = sym_arr[pc];
  prof_arr[pc]++;

  if (prof_arr[pc] > 1) {
    if (sym == NULL) {
      gen_trace_name(tname, TJ);
      gen_so_name(so, tname);
      jit_compile(so, func, pc);
      sym = call_dlfun_arg2(so, func, pc, 3, 3);
    } else {
      printf("found sym\n");
    }
    res = sym(3, 3);
    printf("result: %d\n", res);
  }
  return;
}
