APP = main
DEPS = modcaml.o modwrap.o

$(APP): $(APP).o $(DEPS)
	gcc -m32 -Wall -o $@ $^ -I`ocamlc -where` -L`ocamlc -where` -lm -ldl -lasmrun

%.o: %.c
	gcc -m32 -c $< -I`ocamlc -where` -L`ocamlc -where` -lm -ldl -lasmrun

%caml.o: %.ml
	ocamlopt -output-obj -o $@ $<
