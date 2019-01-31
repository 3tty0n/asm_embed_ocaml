#!/usr/bin/env bash
MINCAML=~/src/github.com/esumii/min-caml

gcc -m32 -c -I"`ocamlc -where`" c_part.c -o c_part.o

ocamlopt -output-obj -o ml_part.o unix.cmxa ml_part.ml

gcc -m32 -c -I"$(ocamlc -where)" mincaml_part.s -o mincaml_part.o

gcc -m32 -o mincaml_part -I"$(ocamlc -where)" -L"$(ocamlc -where)" -lunix -lasmrun -lcurses \
		c_part.o ml_part.o mincaml_part.o \
		~/src/github.com/esumii/min-caml/libmincaml.S \
		~/src/github.com/esumii/min-caml/stub.c
