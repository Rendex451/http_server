#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

typedef enum vtype_t {
    DECIMAL_TYPE,
    REAL_TYPE,
    STRING_TYPE,
} vtype_t;

typedef union value_t {
    int32_t decimal;
    double real;
    char *string;
} value_t;

void *decimal(int32_t x);
void *string(char *x);
void *real(double x);

#endif /* TYPE_H */