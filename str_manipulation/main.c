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

void chars_of_value(char *buf[], value deps, int size) {
  for (int i = 0; i < size; i++) {
    char* str = String_val(Field(deps, i));
    buf[i] = malloc(128 * sizeof(char));
    puts(str);
    strcpy(buf[i], strdup(str));
  }
}

int main(int argc, char *argv[]) {
  char buf[2048];
  value v, v_deps;
  char* tname; int d_size; char* deps[10];

  caml_startup(argv);
  char* arr[] =
    { "tracetj0", "tracetj1", "tracetj2" };

  int n = sizeof(arr)/sizeof(*arr);
  concat(buf, arr, n);

  v = call_caml_f(10);
  tname = strdup(String_val(Field(v, 0)));
  v_deps = Field(v, 1);
  d_size = Int_val(Field(v, 2));

  chars_of_value(deps, v_deps, d_size);

  for(int i = 0; i < d_size; i++) {
    printf("deps[%d] = %s\n", i, deps[i]);
  }

  //puts(jr.trace_name);
  return 0;
}
