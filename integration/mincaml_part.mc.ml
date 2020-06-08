let res1 = call_caml_fib 30 in
let res2 =
  let arg = Array.make 1 (Array.make 0 0) in
  let args = Array.make 4 0 in
  args.(0) <- 100; args.(1) <- 200; args.(2) <- 300; args.(3) <- 400;
  arg.(0) <- args;
  call_caml_add arg
in
print_int res1 ;
print_newline () ;
print_int res2 ;
print_newline ()
