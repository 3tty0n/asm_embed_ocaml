#ifndef MOD_WRAP_H
#define MOD_WRAP_H

struct jit_result {
  char* trace_name;
  char** deps;
  int deps_size;
};

struct jit_result call_caml_f(int n);

#endif
