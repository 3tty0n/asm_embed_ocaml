#!/bin/sh

MINCAML=~/src/github.com/esumii/min-caml

gcc -m32 -S $2.c

gcc -m32 -o prog ${MINCAML}/libmincaml.S ${MINCAML}/stub.c $1.s $2.s
