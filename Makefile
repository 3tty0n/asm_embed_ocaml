CC := gcc
CFLAGS :=
OCAMLOPT := ocamlopt.opt

APP := main

CSRCS := main.c
MLSRCS := ml_part.ml
OBJS := $(CSRCS:%.c=%.o) $(MLSRCS:%.ml=%.o)

.SUFFIXES: .ml .o .c

%.o: %.ml
	$(OCAMLOPT) -output-obj $< -o $(basename $<).o

%.o: %.c
	$(eval ocamlc_where := $(shell ocamlc -where))
	$(CC) -c $< -I$(ocamlc_where)


$(APP): $(OBJS)
	$(eval ocamlc_where := $(shell ocamlc -where))
	$(CC) -o $@ $^ -L$(ocamlc_where) -lm -ldl -lasmrun

clean:
	$(RM) $(APP) $(OBJS)
