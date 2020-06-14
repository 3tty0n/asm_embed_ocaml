#include <caml/compatibility.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/hash.h>
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>

#include <uthash.h>
#include "hash_table.h"

#define THOLD 100

extern void call_test_add(int, int, int) asm("call_test_add");

typedef int (*fun_arg2)(int, int);

enum jit_type {TJ, MJ};

/**
 * For gen_trace_name
 */
int counter = 0;

/**
 * Generate the name of a trace
 * - tracing JIT: tracetj0, tracetj1, ...
 * - method JIT: tracemj0, tracemj1, ...
 */
char *gen_trace_name(enum jit_type typ) {
  char *str;
  if (typ == TJ) {
    sprintf(str, "tracetj%d", counter);
  } else if (typ == MJ) {
    sprintf(str, "tracemj%d", counter);
  } else {
    fprintf(stderr, "invalid jit_type");
    exit(EXIT_FAILURE);
  }
  counter += 1;
  assert(str[41] == '\0');
  return str;
}

int counter_so_name = 0;

char *gen_so_name(enum jit_type typ) {
  char *str;
  if (typ == TJ) {
    sprintf(str, "libtracetj%d.so", counter_so_name);
  } else if (typ == MJ) {
    sprintf(str, "libtracemj%d.so", counter_so_name);
  } else {
    fprintf(stderr, "invalid jit_type");
    exit(EXIT_FAILURE);
  }
  counter_so_name += 1;
  assert(str[41] == '\0');
  return str;
}


int pc_count_arr[300] = {0};
char* pc_name_arr[300] = {NULL};

void count_up(int pc) { pc_count_arr[pc] += 1; }

bool over_thold(int pc) { return pc_count_arr[pc] > 100; }

void register_pc(int pc, enum jit_type typ) {
  char* name;
  name = gen_trace_name(typ);
  pc_name_arr[pc] = name;
}

fun_arg2 sym_arr[100] = {NULL};

int call_dlfun_arg2(char *filename, char *funcname, int pc, int arg1, int arg2) {
  fun_arg2 sym = NULL;
  void *handle = NULL;
  fun_arg2 registered_sym = NULL;
  int res;

  registered_sym = sym_arr[pc];
  if (registered_sym != NULL) {
    res = registered_sym(arg1, arg2);
    return res;
  } else {
    handle = dlopen(filename, RTLD_LAZY);
    if (handle == NULL) {
      fprintf(stderr, "error: dlopen %s\n", filename);
      exit(-1);
    }

    dlerror();

    sym = (fun_arg2)dlsym(handle, funcname);
    if (sym == NULL) {
      fprintf(stderr, "error: dlsym %s\n", funcname);
      exit(-1);
    }
    sym_arr[pc] = sym;
    res = sym(arg1, arg2);

    return res;
  }
}


/**
 * for test
 */
void call_test_add(int pc, int a, int b) {
  char* name;
  name = pc_name_arr[pc];
  if (name == NULL) {
    name = "./libadd.so"; // gen name
    pc_name_arr[pc] = "./libadd.so";
  }
  int x = call_dlfun_arg2(name, "add", pc, a, b);
  //  printf("call_test_add is called: %d + %d = %d\n", a, b, x);
  return;
}
