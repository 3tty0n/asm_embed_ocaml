let f x =
  print_string "f is applied to " ;
  print_int x ;
  print_newline ()

let g x y =
  print_string "g is applied to " ;
  print_int x ;
  print_string " " ;
  print_int y ;
  print_newline ()

let rec fib n = if n < 2 then n else fib (n - 1) + fib (n - 2)

let rec add x y = x + y

let sum arr = Array.fold_left (fun a x -> a + x) 0 arr

let average arr = sum arr / Array.length arr

let () =
  Callback.register "f" f ;
  Callback.register "g" g ;
  Callback.register "fib" fib ;
  Callback.register "average" average ;
  Callback.register "add" add ;
  Callback.register "sum" sum
