let arr = Array.make 10 0 in
arr.(0) <- 1;
arr.(1) <- 2;
arr.(2) <- 3;
call_caml_f (arr.(0)); print_newline ()
