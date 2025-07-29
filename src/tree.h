#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdint.h>

#include "type.h"

typedef struct Tree Tree;

Tree *tree_new(vtype_t key, vtype_t value);
void tree_free(Tree *tree);

value_t tree_get(Tree *tree, void *key);
int8_t tree_set(Tree *tree, void *key, void *value);
void tree_del(Tree *tree, void *key);
_Bool tree_in(Tree *tree, void *key);

_Bool tree_eq(Tree *x, Tree *y);
size_t tree_size(Tree *tree);
extern size_t tree_sizeof(void);

void tree_print(Tree *tree);
void tree_println(Tree *tree);

void tree_print_branches(Tree *tree);
void tree_println_branches(Tree *tree);

#endif /* TREE_H */