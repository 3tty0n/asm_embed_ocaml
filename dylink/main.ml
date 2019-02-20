let _  =
  try
    Dynlink.loadfile "fact.cmxs";
    match !Fact_stub.fact with
      Some f ->
       Printf.printf "%d\n" (f 10)
    | None ->
       failwith "load failed"
  with Dynlink.Error e ->
    print_endline (Dynlink.error_message e)
