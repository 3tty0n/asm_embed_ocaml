open Printf

let f n =
  let others = [|sprintf "tracetj%d" (n+1); sprintf "tracetj%d" (n+2)|] in
  sprintf "tracetj%d" n, others, Array.length others

let _ = Callback.register "caml_f" f
