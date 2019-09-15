#!/bin/bash
ocamlopt -c fact_stub.ml
ocamlopt -o fact.cmxs -shared fact.ml
ocamlopt -o fact-test dynlink.cmxa fact_stub.ml main.ml
