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

static HTTPreq _new_request(void);
static void _parse_request(HTTPreq *request, char *buffer, size_t size);
static void _null_request(HTTPreq *request);
static int8_t _http_switch(HTTP *http, int conn, HTTPreq *request);
static void _http_page404(int conn);

HTTP *http_new(char *address) {
    HTTP *http = (HTTP*)malloc(sizeof(HTTP));
	http->cap = 1000;
	http->len = 0;
	http->host = (char*)malloc(sizeof(char) * strlen(address) + 1);
	strcpy(http->host, address);
	http->map = hashmap_new(http->cap, STRING_TYPE, DECIMAL_TYPE);
	http->funcs = (void (**)(int, HTTPreq*))malloc(http->cap * sizeof(void (*)(int, HTTPreq*)));
	return http;
}

void http_free(HTTP *http) {
	hashmap_free(http->map);
	free(http->host);
	free(http->funcs);
	free(http);
}


int8_t http_listen(HTTP *http) {
	int listener = net_listen(http->host);
	if (listener < 0) {
		return 1;
	}
	for (;;) {
		int conn = net_accept(listener);
		if (conn < 0) {
			continue; // Accept failed, try again
		}
		HTTPreq req = _new_request();
		for (;;) {
			char buffer[BUFSIZ] = {0};
			int n = net_recv(conn, buffer, BUFSIZ - 1);
			if (n < 0) {
				net_close(conn);
				break;
			}
			_parse_request(&req, buffer, n);
			if (n != BUFSIZ - 1) {
				break;
			}
			buffer[BUFSIZ - 1] = '\0';
		}
		_http_switch(http, conn, &req);
		net_close(conn);
	}
	net_close(listener);
    return 0;
}

void http_handle(HTTP *http, char *path, void (*handle)(int, HTTPreq*)) {
	hashmap_set(http->map, string(path), decimal(http->len));
	http->funcs[http->len] = handle;
	http->len++;
	if (http->len >= http->cap) {
		http->cap <<= 1; // =* 2
		http->funcs = (void (**)(int, HTTPreq*))realloc(http->funcs, 
			http->cap * sizeof(void (*)(int, HTTPreq*)));
	}
}

void http_parsehtml(int conn, char *filename) {
	char buffer[BUFSIZ] = 
		"HTTP/1.1 200 OK\n				\
		Content-type: text/html\n\n";	\
	size_t readsize = strlen(buffer);
	net_send(conn, buffer, readsize);
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return;
	}
	while((readsize = fread(buffer, sizeof(char), BUFSIZ, file)) != 0) {
		net_send(conn, buffer, readsize);
	}
	fclose(file);
}

static HTTPreq _new_request() {
	return (HTTPreq) {
		.method = {0},
		.path = {0},
		.proto = {0},
		.state = 0,
		.index = 0
	};
}

static void _parse_request(HTTPreq *request, char *buffer, size_t size) {
	printf("%s\n", buffer);
	for (size_t i = 0; i < size; ++i) {
		switch(request->state) {
			case 0:
				if (buffer[i] == ' ' || request->index == METHOD_SIZE-1) {
					request->method[request->index] = '\0';
					_null_request(request);
					continue;
				}
				request->method[request->index] = buffer[i];
			break;
			case 1:
				if (buffer[i] == ' ' || request->index == PATH_SIZE-1) {
					request->path[request->index] = '\0';
					_null_request(request);
					continue;
				}
				request->path[request->index] = buffer[i];
			break;
			case 2:
				if (buffer[i] == '\n' || request->index == PROTO_SIZE-1) {
					request->proto[request->index] = '\0';
					_null_request(request);
					continue;
				}
				request->proto[request->index] = buffer[i];
			break;
			default: return;
		}
		request->index += 1;
	}
}

static void _null_request(HTTPreq *request) {
	request->state += 1;
	request->index = 0;
}

static int8_t _http_switch(HTTP *http, int conn, HTTPreq *request) {
	if (!hashmap_in(http->map, string(request->path))) {
		char buffer[PATH_SIZE];
		memcpy(buffer, request->path, PATH_SIZE);
		int32_t index = strlen(request->path);
		if (index == 0) {
			_http_page404(conn);
			return 1;
		}
		index -= 1;
//		buffer[index] = '\0';
		for (; index > 0 && buffer[index] != '/'; --index) {
			buffer[index] = '\0';
		}
		if(!hashmap_in(http->map, string(buffer))) {
			_http_page404(conn);
			return 2;
		}
		index = hashmap_get(http->map, string(buffer)).decimal;
		http->funcs[index](conn, request);
		return 0;
	}
	int32_t index = hashmap_get(http->map, string(request->path)).decimal;
	http->funcs[index](conn, request);
	return 0;
}

static void _http_page404(int conn) {
	char *header = "HTTP/1.1 404 Not Found\n\nnot found";
	size_t headsize = strlen(header);
	net_send(conn, header, headsize);
}
