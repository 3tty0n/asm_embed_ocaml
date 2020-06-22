let caml_jit_entry
    bytecode
    stack
    pc
    sp
    sp_ptr
    pc_ptr =
  print_endline "caml_jit_entry is called";
  ()

let () =
  Callback.register "caml_jit_entry" caml_jit_entry;
  ()
