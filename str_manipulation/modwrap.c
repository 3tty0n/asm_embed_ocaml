#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/compatibility.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

struct jit_result {
  char* trace_name;
  char** deps;
  int deps_size;
};

value call_caml_f(int n) {
  value arr, x, y;
  char* xx; char* yy; int z;
  char *buf[10];
  struct jit_result jr;

  return caml_callback(*caml_named_value("caml_f"), Val_int(n)); // ==> : string array array
}
