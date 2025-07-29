#include <stdio.h>
#include <ctype.h>

#include "../src/net.h"

#define BUFF_SIZE 256

int main() {
    int listener = net_listen("0.0.0.0:8080");
    if (listener < 0) {
        fprintf(stderr, "%d\n", listener);
        return 1;
    }

    printf("Server is listening...\n");
    char buffer[BUFF_SIZE];

    for (;;) {
        int conn = net_accept(listener);
        if (conn < 0) {
            fprintf(stderr, "Error: accept\n");
            return 3;
        }
        for (;;) {
            int length = net_recv(conn, buffer, BUFF_SIZE);
            if (length <= 0) {
                break;
            }
            for (char *p = buffer; *p != '\0'; ++p) {
                *p = toupper(*p);
            }
            net_send(conn, buffer, BUFF_SIZE);
        }
        net_close(conn);
    }
}
