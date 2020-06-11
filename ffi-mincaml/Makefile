CC := gcc
CFLAGS := -m32 -O2 -g
OCAMLOPT := ocamlopt
MINCAML := min-caml

APP := prog
DEPS := lib.h
OBJ := interp.s main.o lib.o libmincaml.o modwrap.o modcaml.o runtime.o

$(APP): $(OBJ)
	$(CC) $(CFLAGS) -rdynamic -I`ocamlc -where` -I. -L`ocamlc -where` -L/usr/lib32 $^ -o $@ -lm -ldl -lasmrun

clean:
	$(RM) *.opt *.o *.cm* libadd.so $(APP)

libadd.so: add.o
	$(CC) -m32 -fPIC -shared -Wl,--export-dynamic -o $@ $<

add.o: add.s
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -lm -c $<

%.s: %.ml
	$(MINCAML) $<

%caml.o: %.ml
	$(OCAMLOPT) -output-obj -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -I`ocamlc -where` $^
