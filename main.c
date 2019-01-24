#include <caml/mlvalues.h>
#include <caml/callback.h>

void call_caml_f(int arg) {
  static value * closure_f = NULL;
  if (closure_f == NULL) {
    /* First time around, look up by name */
    closure_f = caml_named_value("test function");
  }
  caml_callback(*closure_f, Val_int(arg));
}

void call_caml_g(int x, int y) {
  static value * closure_g = NULL;
  if (closure_g == NULL) {
    /* First time around, look up by name */
    closure_g = caml_named_value("test function 2");
  }
  caml_callback2(*closure_g, Val_int(x), Val_int(y));
}

int main(int argc, char **argv) {
  caml_main(argv);
  call_caml_f(100);
  call_caml_g(1, 2);
  return 0;
}
