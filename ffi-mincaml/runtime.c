#include <assert.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/compatibility.h>
#include <caml/hash.h>
#include <caml/mlvalues.h>
#include <dlfcn.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <uthash.h>

#define PROF_LEN 100
#define THOLD 100

extern void call_test_add(int, int, int) asm("call_test_add");

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

struct prof {
  int pc;        // key
  int count;     // value 1
  char *so_name; // value 2
};

struct prof *prof_tbl[PROF_LEN] = {NULL};

int prof_tbl_cnt = 0;

void insert_prof(int pc, int count, char *so_name) {
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

struct prof *find_prof(int pc) {
  for (int i = 0; i < PROF_LEN; i++) {
    struct prof *p = prof_tbl[i];
    if (p && p->pc == pc) {
      return p;
    }
  }
  return NULL;
}


struct sym {
  char funcname[128];  // key
  void *handle; // value 2
  fun_arg2 sym; // value 3
  UT_hash_handle hh;
};

struct sym_pc {
  int pc;
  void *handle;
  fun_arg2 sym;
  UT_hash_handle hh;
};

struct sym *syms = NULL;

struct sym_pc *sym_pcs = NULL;

void add_sym(char* funcname, void* handle, fun_arg2 sym) {
  struct sym *s;
  s = malloc(sizeof(struct sym));
  strcpy(s->funcname, funcname);
  s->handle = handle;
  s->sym = sym;
  HASH_ADD_STR(syms, funcname, s);
}

void add_sym_pc(int pc, void *handle, fun_arg2 sym) {
  struct sym_pc *s;
  HASH_FIND_INT(sym_pcs, &pc, s);
  if (s == NULL) {
    s = malloc(sizeof(struct sym_pc));
    HASH_ADD_INT(sym_pcs, pc, s);
  }
  s->pc = pc;
  s->handle = handle;
  s->sym = sym;
}

struct sym *find_sym(char *funcname) {
  struct sym *s;

  HASH_FIND_STR(syms, funcname, s);
  return s;
}

struct sym_pc *find_sym_pc(int pc) {
  struct sym_pc *s;
  HASH_FIND_INT(sym_pcs, &pc, s);

  return s;
}

int call_dlfun_arg2(char *filename, char *funcname, int pc, int arg1,
                    int arg2) {
  fun_arg2 sym = NULL;
  void *handle = NULL;
  int res;

  struct sym_pc *s = find_sym_pc(pc);
  if (s) {
    printf("trace found at pc %d\n", pc);
    sym = s->sym;
    res = sym(arg1, arg2);
    printf("res: %d\n", res);
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
    printf("added sym:\t%s\t%d\n", filename, pc);
    add_sym_pc(pc, handle, sym);
    res = sym(arg1, arg2);

    return res;
  }
}

char* jit_compile(char* func, int pc) {
  char tname[128];
  char so[128];
  char buffer[1024];

  gen_trace_name(tname, TJ);
  gen_so_name(so, tname);

  printf("compiling shared object: %s\n", so);
  sprintf(buffer, "gcc -m32 -fPIC -c %s.s", func);
  system(buffer);
  sprintf(buffer, "gcc -m32 -fPIC -shared -Wl,--export-dynamic -o %s %s.o",
          so, func);
  system(buffer);

  insert_prof(pc, 0, so);
  return strdup(so);
}

/**
 * for test
 */
void call_test_add(int pc, int a, int b) {
  char so[128];
  struct prof *p;

  p = find_prof(pc);
  if (p == NULL) {
    strcpy(so, jit_compile("add", pc));
    int x = call_dlfun_arg2(so, "add", pc, a, b);
    return;
  } else {
    strcpy(so, p->so_name);
    int x = call_dlfun_arg2(so, "add", pc, a, b);
    return;
  }
}

void call_test_sub(int pc, int a, int b) {
  char so[128];
  struct prof *p;

  p = find_prof(pc);
  if (p == NULL) {
    strcpy(so, jit_compile("sub", pc));
    int x = call_dlfun_arg2(so, "sub", pc, a, b);
    return;
  } else {
    strcpy(so, p->so_name);
    int x = call_dlfun_arg2(so, "sub", pc, a, b);
    return;
  }
}
