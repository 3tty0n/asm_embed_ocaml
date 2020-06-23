CC := gcc
CFLAGS := -m32 -O2 -g
OCAMLOPT := ocamlopt
MINCAML := ../min-caml/min-caml

APP := prog
DEPS := lib.h
OBJ := interp.s main.o lib.o libmincaml.o runtime.o modwrap.o modcaml.o
LIBS := mod.a

$(APP): $(OBJ)
	gcc -m32 -g $^ -lasmrun -ldl -lm -rdynamic -o prog -L`ocamlc -where` -I`ocamlc -where`

%.o: %.c
	$(CC) $(CFLAGS) -Wall -Wextra -c $< -I`ocamlc -where`

%.o: %.S
	$(CC) $(CFLAGS) -lm -ldl -c $<

%.s: %.ml
	$(MINCAML) $(basename $<)

%caml.o: %.ml
	$(OCAMLOPT) -output-obj -o $@ $<

.PHONY: clean
clean:
	$(RM) *.opt *.o *.cm* *.so $(APP) $(LIBS)
