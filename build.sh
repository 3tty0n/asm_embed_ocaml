#!/bin/sh

ocamlopt -output-obj ml_part.ml -o ml_part.o

gcc -c main.c  -I"`ocamlc -where`"

gcc -o main.opt  \
    main.o  ml_part.o   \
    -L"`ocamlc -where`" \
    -lm -ldl -lasmrun
