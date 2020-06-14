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


#define PROF_LEN 100
#define THOLD 100

extern void call_test_add(int, int, int) asm("call_test_add");

typedef int (*fun_arg2)(int, int);

enum jit_type {TJ, MJ};

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

struct prof {
  int pc; // key
  int count; // value 1
  char* so_name; // value 2
};

struct prof *prof_tbl[PROF_LEN] = {NULL};

void insert_prof(int pc, int count, char* so_name) {
  for (int i = 0; i < PROF_LEN; i++) {
    if (prof_tbl[i] == NULL) {
      struct prof *p;
      p = malloc(sizeof(struct prof));
      p->pc = pc;
      p->count = count;
      p->so_name = so_name;
      prof_tbl[i] = p;
      return;
    }
  }
}

struct prof* find_prof(int pc) {
  return prof_tbl[pc];
}

struct sym {
  char* so_name; // key
  void* handle; // value 1
  fun_arg2 sym; // value 2
};

struct sym *sym_tbl[PROF_LEN] = {NULL};

void insert_sym(char *so_name, void *handle, fun_arg2 sym) {
  for (int i = 0; i < PROF_LEN; i++) {
    if (sym_tbl[i] == NULL) {
      struct sym *s;
      s = malloc(sizeof(struct sym));
      s->so_name = so_name;
      s->handle = handle;
      s->sym = sym;
      sym_tbl[i] = s;
      return;
    }
  }
}

struct sym *find_sym(char *so_name) {
  for (int i = 0; i < PROF_LEN; i++) {
    struct sym* s = sym_tbl[i];
    if (s != NULL) {
      if (strcmp(s->so_name, so_name) == 0) {
        return s;
      }
    }
  }
  return NULL;
}

int call_dlfun_arg2(char *filename, char *funcname, int pc, int arg1, int arg2) {
  fun_arg2 sym = NULL;
  void *handle = NULL;
  int res;

  struct sym *s = find_sym(filename);
  if (s != NULL) {
    sym = s->sym;
    res = sym(arg1, arg2);
    return res;
  } else {
    handle = dlopen(filename, RTLD_LAZY);
    if (handle == NULL) {
      fprintf(stderr, "error: dlopen %s\n", filename);
      return -1;
    }

    dlerror();

    sym = (fun_arg2)dlsym(handle, funcname);
    if (sym == NULL) {
      fprintf(stderr, "error: dlsym %s\n", funcname);
      return -1;
    }
    insert_sym(filename, handle, sym);
    res = sym(arg1, arg2);

    return res;
  }
}


/**
 * for test
 */
void call_test_add(int pc, int a, int b) {
   char tname[1024]; char so[1024];
  struct prof *p;
  p = find_prof(pc);
  if (p == NULL) {
    char buffer[1024];
    gen_trace_name(tname, TJ);
    gen_so_name(so, tname);
    sprintf(buffer, "gcc -m32 -fPIC -c %s", "add.s");
    system(buffer);
    sprintf(buffer, "gcc -m32 -fPIC -shared -Wl,--export-dynamic -o %s add.o", so);
    system(buffer);
    insert_prof(pc, 0, so);
    int x = call_dlfun_arg2(so, "add", pc, a, b);
    return;
  } else {
    strcpy(so, p->so_name);
    int x = call_dlfun_arg2(so, "add", pc, a, b);
    return;
  }
}
