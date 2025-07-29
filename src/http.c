#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "hashmap.h"
#include "http.h"
#include "net.h"

#define METHOD_SIZE 16
#define PATH_SIZE 2048
#define PROTO_SIZE 16

typedef struct HTTP {
	char *host;
	int32_t len;
	int32_t cap;
	void(**funcs)(int, HTTPreq*);
	HashMap *map;
} HTTP;

HTTP *http_new(char *address)
{
    return nullptr;
}

void http_free(HTTP *http)
{
}


int8_t http_listen(HTTP *http)
{
    return 0;
}

void http_handle(HTTP *http, char *path, void (*)(int, HTTPreq *))
{
}

void http_parsehtml(int conn, char *filename)
{
}
