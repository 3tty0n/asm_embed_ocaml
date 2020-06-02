#include <caml/compatibility.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

value init_f(int n) { return Val_int(n); }

void call_caml_jit_merge_point(int *st, int sp, int *bc, int pc) {
  static value *clsr = NULL;
  value ml_args[6];
  if (clsr == NULL) {
    clsr = caml_named_value("caml_jit_merge_point");
  }
  ml_args[0] = caml_alloc_array(init_f, bc);
  ml_args[1] = caml_alloc_array(init_f, st);
  ml_args[2] = Val_int(pc);
  ml_args[3] = Val_int(sp);
  ml_args[4] = Val_hp(bc);
  ml_args[5] = Val_hp(st);
  caml_callbackN(*clsr, 6, ml_args);
  return;
}
