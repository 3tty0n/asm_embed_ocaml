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

int fib(int n) {
  static value * fib_closure = NULL;
  if (fib_closure == NULL) fib_closure = caml_named_value("fib");
  return Int_val(callback(*fib_closure, Val_int(n)));
}

int array_f(int n) {
	return Val_int(n);
}

int average(int *data) {
  static value * average_closure = NULL;
  if (average_closure == NULL) {
		average_closure = caml_named_value("average");
	}
  return Int_val(callback(*average_closure, alloc_array(array_f, data)));
}
