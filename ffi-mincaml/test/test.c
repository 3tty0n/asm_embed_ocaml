#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROF_LEN 100

int add(int a, int b) { return a + b; }

int sub(int a, int b) { return a - b; }

char *strs[100] = {NULL};

struct prof {
  int pc; // key
  int count; // value 1
  char* so_name; // value 2
};

struct prof *prof_tbl[100] = {NULL};

void insert(int pc, int count, char* so_name) {
  for (int i = 0; i < 100; i++) {
    if (prof_tbl[i] == NULL) {
      struct prof *p;
      p = malloc(sizeof(struct prof));
      p->pc = pc;
      p->count = count;
      p->so_name = so_name;
      prof_tbl[i] = p;
      break;
    }
  }
}

struct prof* find(int pc) {
  return prof_tbl[pc];
}

struct sym {
  char* so_name; // key
  void* handle; // value 1
};

struct sym *sym_tbl[PROF_LEN] = {NULL};

void insert_sym(char *so_name, void *handle) {
  for (int i = 0; i < PROF_LEN; i++) {
    if (sym_tbl[i] == NULL) {
      struct sym *p;
      p = malloc(sizeof(struct sym));
      p->so_name = so_name;
      p->handle = handle;
      sym_tbl[i] = p;
    }
  }
}

struct sym *find_sym(char *so_name) {
  for (int i = 0; i < PROF_LEN; i++) {
    struct sym* s = sym_tbl[i];
    if (s != NULL) {
      printf("s->so_name: %s\n", s->so_name);
      if (strcmp(s->so_name, so_name) == 0) {
        return s;
      }
    }
  }
  return NULL;
}

int main() {
  strs[0] = "abc";
  strs[1] = "def";
  insert(0, 0, "test");
  struct prof *p = find(0);
  printf("p->pc: %d, p->count: %d, p->so_name: %s\n", p->pc, p->count, p->so_name);
  p = find(1);
  if (p == NULL) { puts("null"); }
  else { printf("p->pc: %d, p->count: %d, p->so_name: %s\n", p->pc, p->count, p->so_name); }
  insert_sym("test.so", NULL);
  struct sym *s = find_sym("test.so");
  if (s == NULL) {
    return -1;
  }
  return 0;
}
