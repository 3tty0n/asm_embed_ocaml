#include <stdio.h>
#include <dlfcn.h>
#include <caml/mlvalues.h>

typedef int (*dylink_fun)(int);

CAMLprim value call_caml_dlopen(value filename, value funcname) {
  dylink_fun sym = NULL;
  void *handle = NULL;

  handle = dlopen(String_val(filename), RTLD_LAZY);

  if (handle == NULL) {
    fprintf(stderr, "error: dlopen\n");
    return -1;
  }

  sym = (dylink_fun)dlsym(handle, String_val(funcname));

  if (sym == NULL) {
    fprintf(stderr, "error: dlsym\n");
    return -1;
  }

  printf("%d\n", sym(10));

  return Val_unit;
}
