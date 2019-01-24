let f x =
  print_string "f is applied to ";
  print_int x;
  print_newline()

let g x y =
  print_string "g is applied to ";
  print_int x;
  print_string " ";
  print_int y;
  print_newline ()

let () =
  Callback.register "test function" f;
  Callback.register "test function 2" g
