CC := gcc
CFLAGS :=
OCAMLC := ocamlopt

ifdef 32bit
	CFLAGS += -m32
endif

APP := main

CSRCS := main.c
MLSRCS := ml_part.ml
OBJS := $(CSRCS:%.c=%.o) $(MLSRCS:%.ml=%.o)

.SUFFIXES: .ml .o .c

%.o: %.ml
	$(OCAMLC) -output-obj $< -o $(basename $<).o

%.o: %.c
	$(eval ocamlc_where := $(shell ocamlc -where))
	$(CC) $(CFLAGS) -c $< -I$(ocamlc_where)


$(APP): $(OBJS)
	$(eval ocamlc_where := $(shell ocamlc -where))
	$(CC) $(CFLAGS) -o $@ $^ -L$(ocamlc_where) -lm -ldl

clean:
	$(RM) $(APP) $(OBJS) *.cm*
