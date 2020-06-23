#include <caml/compatibility.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

value init_f(int x) { return Val_int(x); }

int fib(int n) {
  static const value * fib_closure = NULL;
  if (fib_closure == NULL) fib_closure = caml_named_value("fib");
  return Int_val(caml_callback(*fib_closure, Val_int(n)));
}

int jit_entry(int *stack, int sp, int *code, int pc) {
  static const value *closure = NULL;
  value ml_args[4];

  if (closure == NULL) closure = caml_named_value("jit_entry");

  ml_args[0] = alloc_array(init_f, stack);
  ml_args[1] = alloc_array(init_f, code);
  ml_args[2] = Val_int(sp);
  ml_args[3] = Val_int(pc);

  return Int_val(caml_callbackN(closure, 4, ml_args));
}

value bytecode = NULL;

void call_caml_jit_exec(int *stack, int sp, int *code, int pc) {
  value res;
  value ml_args[6];

  static const *jit_entry_clsr = NULL;
  if (jit_entry_clsr == NULL) caml_named_value("caml_jit_entry");

  ml_args[0] = caml_alloc_array(init_f, code);
  ml_args[1] = caml_alloc_array(init_f, stack);
  ml_args[2] = Val_int(pc);
  ml_args[3] = Val_int(sp);
  ml_args[4] = Val_hp(stack);
  ml_args[5] = Val_hp(code);
  res = caml_callbackN(jit_entry_clsr, 6, ml_args);
  return;
}
