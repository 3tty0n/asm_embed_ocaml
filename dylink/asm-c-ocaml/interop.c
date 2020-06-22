#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>


extern void call_caml_jit_merge_point(int *, int, int *, int);

value init_f(int n) { return Val_int(n); }

void call_caml_jit_merge_point(int *stk, int sp, int *bytecode, int pc) {
  static value *clsr = NULL;
  value ml_args[6];
  if (clsr == NULL) {
    clsr = caml_named_value("jit_merge_point");
  }
  ml_args[0] = caml_alloc_array(init_f, bytecode);
  ml_args[1] = caml_alloc_array(init_f, stk);
  ml_args[2] = Val_int(pc);
  ml_args[3] = Val_int(sp);
  ml_args[4] = Val_hp(bytecode);
  ml_args[5] = Val_hp(stk);
  caml_callbackN(*clsr, 6, ml_args);
  return;
}
