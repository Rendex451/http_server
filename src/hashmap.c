#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "hashmap.h"
#include "tree.h"


typedef struct HashMap {
    struct {
        vtype_t key;
        vtype_t value;
    } type;
    size_t size;
    Tree **table;
} HashMap;

static uint32_t _get_hash(HashMap *hashmap, void *key);
static uint32_t _strhash(char *s, size_t size);

HashMap *hashmap_new(size_t size, vtype_t key, vtype_t value) {
    switch(key){
        case DECIMAL_TYPE:
        case STRING_TYPE:
            break;
        default:
            fprintf(stderr, "%s\n", "key type not supported");
            return NULL;
    }
    switch(value) {
        case DECIMAL_TYPE: 
        case REAL_TYPE: 
        case STRING_TYPE: 
            break;
        default:
            fprintf(stderr, "%s\n", "value type not supported");
            return NULL;
    }
    HashMap *hashmap = (HashMap*)malloc(sizeof(HashMap));
    hashmap->table = (Tree**)malloc(size * sizeof(Tree*));
    for (size_t i = 0; i < size; ++i) {
        hashmap->table[i] = tree_new(key, value);
    }
    hashmap->size = size;
    hashmap->type.key = key;
    hashmap->type.value = value;
    return hashmap;
}

void hashmap_del(HashMap *hashmap, void *key) {
    uint32_t hash = _get_hash(hashmap, key);
    tree_del(hashmap->table[hash], key);
}

_Bool hashmap_in(HashMap *hashmap, void *key) {
    uint32_t hash = _get_hash(hashmap, key);
    return tree_in(hashmap->table[hash], key);
}

value_t hashmap_get(HashMap *hashmap, void *key) {
    uint32_t hash = _get_hash(hashmap, key);
    return tree_get(hashmap->table[hash], key);
}

int8_t hashmap_set(HashMap *hashmap, void *key, void *value) {
    uint32_t hash = _get_hash(hashmap, key);
    tree_set(hashmap->table[hash], key, value);
    return 0;
}

_Bool hashmap_eq(HashMap *x, HashMap *y) {
    if (x->type.key != y->type.key) {
        return 0;
    }
    if (x->type.value != y->type.value) {
        return 0;
    }
    if (x->size != y->size) {
        return 0;
    }
    for (size_t i = 0; i < x->size; ++i) {
        if (tree_size(x->table[i]) != tree_size(y->table[i])) {
            return 0;
        }
        if (tree_eq(x->table[i], y->table[i]) != 0) {
            return 0;
        }
    }
    return 1;
}

size_t hashmap_size(HashMap *hashmap) {
    return hashmap->size;
}

size_t hashmap_sizeof(void) {
    return sizeof(HashMap);
}

void hashmap_free(HashMap *hashmap) {
    for (size_t i = 0; i < hashmap->size; ++i) {
        tree_free(hashmap->table[i]);
    }
    free(hashmap->table);
    free(hashmap);
}

void hashmap_print(HashMap *hashmap) {
    printf("#H[ ");
    for (size_t i = 0; i < hashmap->size; ++i) {
        if (tree_size(hashmap->table[i]) == 0) {
            continue;
        }
        printf("(%u :: ", (uint32_t)i);
        tree_print(hashmap->table[i]);
        printf(") ");
    }
    putchar(']');
}

void println_hashmap(HashMap *hashmap) {
    hashmap_print(hashmap);
    putchar('\n');
}

void hashmap_print_format(HashMap *hashmap) {
    printf("#H[\n");
    for (size_t i = 0; i < hashmap->size; ++i) {
        if (tree_size(hashmap->table[i]) == 0) {
            continue;
        }
        printf("\t(%u :: ", (uint32_t)i);
        tree_print(hashmap->table[i]);
        printf(")\n");
    }
    putchar(']');
}

void hashmap_println_format(HashMap *hashmap) {
    hashmap_print_format(hashmap);
    putchar('\n');
}

static uint32_t _get_hash(HashMap *hashmap, void *key) {
    uint32_t hash = 0;
    switch(hashmap->type.key) {
        case DECIMAL_TYPE:
            hash = (uint32_t)(intptr_t)key % hashmap->size;
        break;
        case STRING_TYPE:
            hash = _strhash((char*)key, hashmap->size);
        break;
        default: ;
    }
    return hash;
}

static uint32_t _strhash(char *s, size_t size) {
    uint32_t hashval;
    for (hashval = 0; *s != '\0'; ++s) {
        hashval = *s + 31 * hashval;
    }
    return hashval % size;
}