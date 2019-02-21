#!/bin/sh
gcc -m32 -dynamiclib -o libshared.dylib loadee.c
ocamlopt -o main.opt main.ml loader.c
