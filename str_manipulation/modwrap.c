#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/compatibility.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

struct jit_result {
  char* trace_name;
  char** deps;
  int deps_size;
};

void chars_of_value(char *buf[], value deps, int size) {
  for (int i = 0; i < size; i++) {
    char* str = strdup(String_val(Field(deps, i)));
    buf[i] = malloc(128 * sizeof(char));
    puts(str);
    strcpy(buf[i], str);
  }
}


struct jit_result call_caml_f(int n) {
  value arr, x, y;
  char* xx; char* yy; int z;
  char *buf[10];
  struct jit_result jr;

  arr = caml_callback(*caml_named_value("caml_f"), Val_int(n)); // ==> : string array array
  xx = String_val(Field(arr, 0)); // "str1"
  puts(xx);

  //buf[0] = malloc(strlen(xx) * sizeof(char));
  //strcpy(buf[0], xx);

  y = Field(arr, 1); // [|"str2"; "str3"; ...|]
  z = Int_val(Field(arr, 2));

  chars_of_value(buf, y, z);

  //printf("size %d\n", z);
  //yy = strdup(String_val(Field(y, 0)));
  //puts(yy);
  //buf[1] = malloc(128 * sizeof(char));
  //strcpy(buf[1], yy);

  jr.trace_name = malloc(128 * sizeof(char));
  strcpy(jr.trace_name, xx);
  jr.deps = buf;
  jr.deps_size = z;

  return jr;
  //return arr[0];
}
