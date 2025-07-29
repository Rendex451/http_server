#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdint.h>

#include "type.h"

typedef struct HashMap HashMap;

HashMap *hashmap_new(size_t size, vtype_t key, vtype_t value);
void hashmap_free(HashMap *hashmap);

value_t hashmap_get(HashMap *hashmap, void *key);
int8_t hashmap_set(HashMap *hashmap, void *key, void *value);
void hashmap_del(HashMap *hashmap, void *key);
_Bool hashmap_in(HashMap *hashmap, void *key);

_Bool hashmap_eq(HashMap *x, HashMap *y);
size_t hashmap_size(HashMap *hashmap);
size_t hashmap_sizeof(void);

void hashmap_print(HashMap *hashmap);
void hashmap_println(HashMap *hashmap);

void hashmap_print_format(HashMap *hashmap);
void hashmap_println_format(HashMap *hashmap);

#endif /* HASHMAP_H */