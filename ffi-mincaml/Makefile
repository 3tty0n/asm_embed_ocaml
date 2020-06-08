CC := gcc
CFLAGS := -m32
OCAMLOPT := ocamlopt
MINCAML := min-caml

APP := prog

$(APP): interp.s main.o lib.o libmincaml.o modwrap.o modcaml.o
	$(CC) $(CFLAGS) -I`ocamlc -where` $^ -o $@ -L`ocamlc -where` -lm -ldl -lasmrun

clean:
	$(RM) *.opt *.o $(APP)

%.o: %.S
	$(CC) $(CFLAGS) -lm -c $<

%.s: %.ml
	$(MINCAML) $<

%.o: %.c
	$(OCAMLOPT) -c -o $@

%caml.o: %.ml
	$(OCAMLOPT) -output-obj -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -I`ocamlc -where` $^
