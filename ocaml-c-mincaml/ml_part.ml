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

let rec fib n =
  if n < 2 then n
  else fib (n - 1) + fib (n - 2)

let () =
  Callback.register "f" f;
  Callback.register "g" g;
  Callback.register "fib" fib;
