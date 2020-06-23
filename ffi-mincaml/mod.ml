let rec fib n = if n < 2 then 1 else fib(n-1) + fib(n-2)

let caml_jit_entry
    bytecode
    stack
    pc
    sp
    sp_ptr
    pc_ptr =
  print_endline "caml_jit_entry is called";
  ()

let _ = Callback.register "fib" fib
let _ = Callback.register "caml_jit_entry" caml_jit_entry
