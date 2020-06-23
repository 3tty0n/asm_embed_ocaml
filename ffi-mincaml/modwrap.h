#ifndef MODWRAP_DOT_H
#define MODWRAP_DOT_H

int fib(int);

int call_can_enter_jit(int *, int, int *, int);

void jit_entry(int*, int, int*, int);

#endif /* MODWRAP_DOT_H */
