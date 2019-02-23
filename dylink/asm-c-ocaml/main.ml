external call_dlfun_arg1 : string -> string -> int -> unit = "call_dlfun_arg1"
external call_dlfun_arg2 : string -> string -> int -> int -> unit = "call_dlfun_arg2"

let () =
  call_dlfun_arg2 "./libadd.dylib" "add" 100 11
