CC := gcc
CFLAGS := -m32
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
	$(CC) $(CFLAGS) -o $@ $^ -L`ocamlc -where` -I`ocamlc -where` -lm -ldl -lasmrun

clean:
	$(RM) $(APP) $(OBJS) *.cm*
