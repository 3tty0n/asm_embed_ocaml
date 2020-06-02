let caml_jit_merge_point
    bytecode
    stack
    pc
    sp
    sp_ptr
    pc_ptr =
  ()

let () =
  Callback.register "caml_jit_merge_point" caml_jit_merge_point;
  ()
