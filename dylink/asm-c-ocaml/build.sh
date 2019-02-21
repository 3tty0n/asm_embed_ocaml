#!/bin/sh
gcc -dynamiclib -o libshared.dylib loadee.c
ocamlopt -o main.opt main.ml loader.c
