external call_caml_dlopen : string -> unit = "call_caml_dlopen"

let () =
  call_caml_dlopen "./libshared.dylib"
