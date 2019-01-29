CC := gcc
CFLAGS := -m32
OCAMLC := ocamlopt

CSRCS := c_part.c
MLSRCS := ml_part.ml
APP := mincaml_part
OBJS := $(CSRCS:%.c=%.o) $(MLSRCS:%.ml=%.o) $(APP).o

OCAMLC_WHERE := $(shell ocamlc -where)
MINCAML_WHERE := ~/src/github.com/esumii/min-caml
MINCAML_STUBS := $(MINCAML_WHERE)/libmincaml.S $(MINCAML_WHERE)/stub.c

.SUFFIXES: .ml .o .c

%.o: %.s
	$(CC) $(CFLAGS) -c -I"$(OCAMLC_WHERE)" -o $@ $<

%.o: %.ml
	$(OCAMLC) -output-obj -o $@ unix.cmxa $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -I$(OCAMLC_WHERE) -o $@


$(APP): $(APP).o $(OBJS)
	$(CC) $(CFLAGS) -o $@ -I$(OCAMLC_WHERE) -L$(OCAMLC_WHERE) $(MINCAML_STUBS) -lunix -lasmrun -lcurses $^

clean:
	$(RM) $(OBJS) $(APP) *.cm*
