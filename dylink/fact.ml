let rec fact =
  function
    0 -> 1
  | n -> n * fact (n-1)

let _ =
  Fact_stub.fact := Some fact
