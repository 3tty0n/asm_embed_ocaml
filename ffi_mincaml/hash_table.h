typedef int (*fun_arg2)(int, int);

typedef struct {
  char* key;
  fun_arg2 value;
} ht_item;

typedef struct {
  int size;
  int count;
  ht_item** items;
} ht_hash_table;

ht_hash_table* ht_new();
void ht_insert(ht_hash_table* ht, const char* key, const fun_arg2 value);
fun_arg2 ht_find(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);
