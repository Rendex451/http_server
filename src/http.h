#ifndef HTTP_H
#define HTTP_H

#include <stdint.h>
#include <stddef.h>

typedef struct HTTPreq {
    char method[16];
    char path[2048];
    char proto[16];
    uint8_t state;
    size_t index;
} HTTPreq;

typedef struct HTTP HTTP;

HTTP* http_new(char *address);
void http_free(HTTP *http);

int8_t http_listen(HTTP *http);
void http_handle(HTTP *http, char *path, void(*handle)(int, HTTPreq*));
void http_parsehtml(int conn, char *filename);

#endif /* HTTP_H */
