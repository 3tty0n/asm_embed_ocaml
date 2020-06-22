#define _GNU_SOURCE

#include <assert.h>
#define CAML_NAME_SPACE
#include <caml/alloc.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/callback.h>
#include <caml/compatibility.h>
#include <dlfcn.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <uthash.h>

#include "modwrap.h"

#define PROF_LEN 100
#define THOLD 100

#define JIT_COMPILE_COMMAND "gcc -m32 -fPIC"

extern int call_test_add(int, int, int) asm("call_test_add");
extern int call_test_sub(int, int, int) asm("call_test_sub");

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
  int pc;             // key
  int count;          // value 1
  char so_name[1024]; // value 2
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
      strcpy(p->so_name, so_name);
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
  char filename[1024]; // key
  char funcname[1024]; // value 1
  fun_arg2 sym;        // value 2
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

void add_sym(char *filename, char *funcname, fun_arg2 sym) {
  struct sym *s;
  s = malloc(sizeof(struct sym));
  strcpy(s->filename, filename);
  strcpy(s->funcname, funcname);
  s->sym = sym;
  HASH_ADD_STR(syms, filename, s);
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

fun_arg2 sym_arr[2048] = {NULL};

int call_dlfun_arg2(char *filename, char *funcname, int pc, int arg1,
                    int arg2) {
  fun_arg2 sym = NULL;
  void *handle = NULL;
  int res;

  sym = sym_arr[pc];
  if (sym) {
    printf("trace found at pc %d %s\n", pc, funcname);
    res = sym(arg1, arg2);
    printf("res: %d\n", res);
    return res;
  } else {
    handle = dlopen(filename, RTLD_LAZY | RTLD_GLOBAL);
    if (handle == NULL) {
      fprintf(stderr, "error: dlopen %s\n", filename);
      return -1;
    }
    dlerror();

    sym = (fun_arg2)dlsym(RTLD_DEFAULT, funcname);
    if (sym == NULL) {
      fprintf(stderr, "error: dlsym \n");
      return -1;
    }
    sym_arr[pc] = sym;
    printf("added sym:\t%s\t%d\n", filename, pc);
    res = sym(arg1, arg2);

    return res;
  }
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

int call_jit_merge_point(int *stack, int sp, int *code, int pc) {
  char tname[1024];
  char so[1024];
  char func[10];
  struct prof *p;
  printf("pc: %d\n", pc);

  if (pc == 0 || pc == 4)
    strcpy(func, "add");
  else if (pc == 2 || pc == 5)
    strcpy(func, "sub");
  else
    return 0;

  p = find_prof(pc);
  if (p == NULL) {
    gen_trace_name(tname, TJ);
    gen_so_name(so, tname);
    jit_compile(so, func, pc);
    insert_prof(pc, 0, so);
    int x = call_dlfun_arg2(so, func, pc, 3, 3);
    return x;
  } else {
    strcpy(so, p->so_name);
    int x = call_dlfun_arg2(so, func, pc, 3, 3);
    return x;
  }
  return 0;
}

value init_f(int x) { return Val_int(x); }

value bytecode = NULL;

void call_caml_jit_exec(int *stack, int sp, int *code, int pc) {
  CAMLparam4(stack, sp, code, pc);
  value res;
  value ml_args[6];

  static const *jit_entry_clsr = NULL;
  if (jit_entry_clsr == NULL)
    caml_named_value("caml_jit_entry");

  if (bytecode == NULL)
    bytecode = caml_alloc_array(init_f, code);

  ml_args[0] = caml_alloc_array(init_f, stack);
  ml_args[1] = Val_int(sp);
  ml_args[2] = bytecode;
  ml_args[3] = Val_int(pc);
  ml_args[4] = Val_hp(stack);
  ml_args[5] = Val_hp(code);
  res = caml_callbackN(jit_entry_clsr, 6, ml_args);
  return;
}
