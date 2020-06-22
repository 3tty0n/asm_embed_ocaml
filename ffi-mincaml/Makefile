CC := gcc
CFLAGS := -m32 -O2 -g
OCAMLOPT := ocamlopt
MINCAML := min-caml

APP := prog
DEPS := lib.h
OBJ := interp.s main.o lib.o libmincaml.o runtime.o modcaml.o
LIBS := mod.a

$(APP): $(OBJ)
	gcc -m32 -g $^ -lasmrun -ldl -lm -o prog -L`ocamlc -where` -I`ocamlc -where` -rdynamic

%.o: %.c
	$(CC) $(CFLAGS) -Wall -Wextra -c $< -I`ocamlc where`

clean:
	$(RM) *.opt *.o *.cm* *.so $(APP) $(LIBS)

%.o: %.S
	$(CC) $(CFLAGS) -lm -ldl -c $<

%.s: %.ml
	$(MINCAML) $<

%caml.o: %.ml
	$(OCAMLOPT) -output-obj -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -I`ocamlc -where` $^
