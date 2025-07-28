#include <string.h>
#include "http.h"

void index_page(int conn, HTTPreq *request);
void about_page(int conn, HTTPreq *request);

int main(void) {
    HTTP *serve = new_http("127.0.0.1:7575");

    handle_http(serve, "/", index_page);
    handle_http(serve, "/about", about_page);

    listen_http(serve);
    free_http(serve);

    return 0;
}

void index_page(int conn, HTTPreq *request) {
    if (strcmp(req->path, "/") != 0) {
        parsehtml_http(conn, "index.html");
        return;
    }
    parsehtml_http(conn, "index.html");
}
