#include <stdio.h>
#include <string.h>
#include <caml/callback.h>

#include "modwrap.h"

void concat(char* buf, char** arr, int size) {
  strcpy(buf, "");

  for (int i = 0; i < size; i++) {
    strcat(buf, "-l");      // -l
    strcat(buf, arr[i]); // -lfoo
    strcat(buf, " ");
  }

}

int main(int argc, char *argv[]) {
  char buf[2048];
  char* tr_name; char** deps;
  struct jit_result jr;

  caml_startup(argv);
  char* arr[] =
    { "tracetj0", "tracetj1", "tracetj2" };

  int n = sizeof(arr)/sizeof(*arr);
  concat(buf, arr, n);

  jr = call_caml_f(10);
  strcpy(tr_name, strdup(jr.trace_name));
  deps = jr.deps;
  printf("deps size %d\n", jr.deps_size);

  //puts(jr.trace_name);
  return 0;
}
