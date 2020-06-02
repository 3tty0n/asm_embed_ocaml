(* let rec jit_merge_point _ _ _ _ = () ;; *)

let rec interp stack sp code pc =
  jit_merge_point stack sp code pc;
  let opcode = code.(pc) in
  if opcode = 1 then            (* ADD *)
    let v1 = stack.(sp - 1) in
    let v2 = stack.(sp - 2) in
    stack.(sp - 2) <- v1 + v2;
    interp stack (sp - 1) code (pc + 1)
  else if opcode = 2 then       (* SUB *)
    let v1 = stack.(sp - 1) in
    let v2 = stack.(sp - 2) in
    stack.(sp - 2) <- v1 - v2;
    interp stack (sp - 1) code (pc + 1)
  else if opcode = 3 then       (* LT *)
    let v2 = stack.(sp - 1) in
    let v1 = stack.(sp - 2) in
    let n = if v1 < v2 then 1 else 0 in
    stack.(sp - 2) <- n;
    interp stack (sp - 1) code (pc + 1)
  else if opcode = 4 then       (* JUMP_IF_ZERO *)
    let addr = code.(pc + 1) in
    let sp2 = sp - 1 in
    let v = stack.(sp2) in
    if v = 0 then (
      interp stack sp2 code addr
    ) else
      interp stack sp2 code (pc + 2)
  else if opcode = 5 then       (* CONST *)
    let n = code.(pc + 1) in
    stack.(sp - 1) <- n;
    interp stack (sp + 1) code (pc + 2)
  else if opcode = 9 then       (* HALT *)
    stack.(sp - 1)
  else
    -1000
;;

let stack = Array.make 50 (-1) in
let code = [|5; 1; 5; 2; 1; 9|] in
(* let res = interp stack 0 code 0 in print_int res *)
let rec loop i =
  if i = 0 then ()
  else
    let _ = interp stack 0 code 0 in
    loop (i-1)
in
let s = get_current_micros () in
let _ = loop 100 in
let e = get_current_micros () in
print_int (e - s)
