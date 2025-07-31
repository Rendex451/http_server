#include <stdio.h>
#include <string.h>

#include "http.h"

const char *page_dir = "../pages/";

void index_page(int conn, HTTPreq *request);
void about_page(int conn, HTTPreq *request);

int main(void) {
	HTTP *serve = http_new("127.0.0.1:7575");

	http_handle(serve, "/", index_page);
	http_handle(serve, "/about/", about_page);

	http_listen(serve);
	http_free(serve);

	return 0;
}

void index_page(int conn, HTTPreq *req) {
	printf("%s - %s - %s\n", req->method, req->path, req->proto);
	char filepath[256];
	if(strcmp(req->path, "/") != 0) {
		snprintf(filepath, sizeof(filepath), "%spage404.html", page_dir);
		http_parsehtml(conn, filepath);
		return;
	}
	snprintf(filepath, sizeof(filepath), "%sindex.html", page_dir);
	http_parsehtml(conn, filepath);
}

void about_page(int conn, HTTPreq *req) {
	char filepath[256];
	if(strcmp(req->path, "/about/") != 0) {
		snprintf(filepath, sizeof(filepath), "%spage404.html", page_dir);
		http_parsehtml(conn, filepath);
		return;
	}
	snprintf(filepath, sizeof(filepath), "%sabout.html", page_dir);
	http_parsehtml(conn, filepath);
}

// #include <stdio.h>
// #include <string.h>

// #include "http.h"

// void index_page(int conn, HTTPreq *req);
// void about_page(int conn, HTTPreq *req);

// int main(void) {
// 	HTTP *serve = http_new("127.0.0.1:7545");

// 	http_handle(serve, "/", index_page);
// 	http_handle(serve, "/about/", about_page);

// 	http_listen(serve);
// 	http_free(serve);
// 	return 0;
// }

// void index_page(int conn, HTTPreq *req) {
// 	if(strcmp(req->path, "/") != 0) {
// 		http_parsehtml(conn, "page404.html");
// 		return;
// 	}
// 	http_parsehtml(conn, "index.html");
// }

// void about_page(int conn, HTTPreq *req) {
// 	if(strcmp(req->path, "/about/") != 0) {
// 		http_parsehtml(conn, "page404.html");
// 		return;
// 	}
// 	http_parsehtml(conn, "about.html");
// }