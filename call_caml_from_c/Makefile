APP := main
CC := gcc -m32
OCAMLOPT := ocamlopt

$(APP): $(APP).o modwrap.o modcaml.o
	$(CC) $^ -o $@ -lm -ldl -L`ocamlc -where` -lasmrun -I`ocamlc -where`

%.o: %.c
	$(CC) -c $< -I`ocamlc -where`

%caml.o: %.ml
	$(OCAMLOPT) -output-obj -o $@ $<

clean:
	$(RM) *.cm* *.o
