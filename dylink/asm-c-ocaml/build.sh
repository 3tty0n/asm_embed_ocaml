#!/bin/sh
gcc -m32 -fPIC -shared -ldl -lm add.s libmincaml.S -o libadd.so
gcc -m32 -c -I`ocamlc -where` loader_stub.c
ocamlopt -c loader.ml loader_stub.o
