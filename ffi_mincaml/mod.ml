let rec fib n = if n < 2 then 1 else fib(n-1) + fib(n-2)

let rec jit_entry stack sp code pc =
  print_string "pc in caml: "; print_int pc; print_newline ();
  ()

let _ = Callback.register "fib" fib
let _ = Callback.register "jit_entry" jit_entry
