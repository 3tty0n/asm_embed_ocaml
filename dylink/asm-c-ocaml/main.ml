external call_caml_dlopen : string -> string -> unit = "call_caml_dlopen"

let () =
  call_caml_dlopen "./libshared.dylib" "fact";
  call_caml_dlopen "./libsum.dylib" "add"
