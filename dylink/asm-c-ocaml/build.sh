#!/bin/sh
gcc -m32 -fPIC -shared -ldl add.s -o libadd.so
ocamlopt -o main.opt main.ml loader.o
