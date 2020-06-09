#include <caml/compatibility.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <uthash.h>
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>

#define THOLD 100

enum jit_type {TJ, MJ};

int counter = 0;
n
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
	printf(stderr, "invalid jit_type");
	exit(EXIT_FAILURE);
  }
  counter += 1;
  assert(str[41] == '\0');
  return str;
}

struct prof_tbl {
  int pc; // key
  int count; // value
  UT_hash_handle hh; // make it hashable
};

struct prof_tbl *prof_tbl = NULL;

void add_pc(int new_pc) {
  struct prof_tbl *p;
  p = malloc(sizeof(struct prof_tbl));
  p->pc = new_pc;
  p->count = 1;
  HASH_ADD_INT(prof_tbl, pc, p);
  return;
}

void count_up(int pc) {
  struct prof_tbl *p;
  HASH_FIND_INT(prof_tbl, &pc, p);
  if (p == NULL) {
    p = (struct prof_tbl *)malloc(sizeof *p);
    p->pc = pc;
    p->count = 1;
  } else {
    p->count = p->count += 1;
  }
  HASH_ADD_INT(prof_tbl, pc, p);
  return;
}

bool over_thold(int pc) {
  struct prof_tbl *p;
  HASH_FIND_INT(prof_tbl, &pc, p);
  if (p == NULL) return false;
  return p->count > THOLD;
}

typedef int (*fun_arg2)(int, int);

int call_dlfun_arg1(char *filename, char *funcname, int arg1, int arg2) {
  fun_arg2 sym = NULL;
  void *handle = NULL;
  int res;

  handle = dlopen(filename, RTLD_LAZY);
  if (handle == NULL) {
    fprintf(stderr, "error: dlopen:\n");
    exit(-1);
  }

  sym = (fun_arg2)dlsym(handle, funcname);
  if (sym == NULL) {
    fprintf(stderr, "error: dlsym:\n");
    exit(-1);
  }

  res = sym(arg1, arg2);
  return res;
}
