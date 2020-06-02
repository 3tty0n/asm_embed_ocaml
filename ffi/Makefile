CC := gcc
CFLAGS := -m32
OCAMLOPT := ocamlopt
MINCAML := min-caml

prog: interp.s main.o libmincaml.o modwrap.o modcaml.o
	$(CC) $(CFLAGS) -I`ocamlc -where` $^ -o $@ -L`ocamlc -where` -lm -ldl -lasmrun

clean:
	$(RM) *.opt *.o

libmincaml.o: libmincaml.S
	$(CC) $(CFLAGS) -lm -c $<

interp.s: interp.ml
	$(MINCAML) $<

modwrap.o: modwrap.c
	$(OCAMLOPT) -c -o $@

modcaml.o: mod.ml
	$(OCAMLOPT) -output-obj -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -I`ocamlc -where` $^
