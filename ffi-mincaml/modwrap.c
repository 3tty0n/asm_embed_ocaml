#include <caml/compatibility.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>

value init_f(int x) { return Val_int(x); }

value bytecode = NULL;

void call_caml_jit_exec(int *stack, int sp, int *code, int pc) {
  CAMLparam4(stack, sp, code, pc);
  value res;
  value ml_args[6];
  CAMLLocal(res, ml_args);

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
