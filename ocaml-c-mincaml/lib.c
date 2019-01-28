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
  static value *closure_f = NULL;
  if (closure_f == NULL) {
    closure_f = caml_named_value("test function");
  }
  caml_callback(*closure_f, Val_int(arg));
}

void run_caml_f(int arg, char** argv) {
	call_caml_f(arg);
}
