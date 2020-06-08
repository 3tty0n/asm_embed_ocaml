#include <stdio.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>

int f(int x) {
  int y = x + 10;
  return y;
}

int g(int x, int y) {
  return x + y;
}

int h(int *data) {
  return data[1] + 100;
}

void call_caml_f(int arg) {
  static value * closure_f = NULL;
  if (closure_f == NULL) {
    /* First time around, look up by name */
    closure_f = caml_named_value("f");
  }
  caml_callback(*closure_f, Val_int(arg));
}

int call_caml_fib(int n) {
  value args[1];
  static value * fib_closure = NULL;
  if (fib_closure == NULL) fib_closure = caml_named_value("fib");
  args[0] = Val_int(n);
  return Int_val(caml_callbackN(*fib_closure, 1, args));
}

int call_caml_add(int **x) {
  value args[4];
  static value * add_clsr = NULL;
  if (add_clsr == NULL) {
    add_clsr = caml_named_value("add");
  }
  args[0] = Val_int(x[0][0]);
  args[1] = Val_int(x[0][1]);
  args[2] = Val_int(x[0][2]);
  args[3] = Val_int(x[0][3]);
  return Int_val(caml_callbackN(*add_clsr, 4, args));
}

int init_f(int n) {
  return Val_int(n);
}

int call_caml_average(int *data) {
  static value * average_closure = NULL;
  if (average_closure == NULL) {
    average_closure = caml_named_value("average");
  }
  return Int_val(callback(*average_closure, caml_alloc_array(init_f, data)));
}
