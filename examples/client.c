#include <stdio.h>
#include <stdlib.h>

#include "../src/net.h"

#define BUFF_SIZE 256
#define INDEX(ptr, init) (ptr-init)

void client_input(char *buffer, size_t size);

int main() {
    int conn = net_connect("127.0.0.1:8080");
    if (conn < 0) {
        fprintf(stderr, "err: %d\n", conn);
        return 1;
    }

    char buff[BUFF_SIZE];
    client_input(buff, BUFF_SIZE);

    net_send(conn, buff, BUFF_SIZE);
    net_recv(conn, buff, BUFF_SIZE);

    printf("%s\n", buff);
    net_close(conn);

    return 0;
}

void client_input(char *buffer, size_t size) {
    if (size == 0) {
        return;
    }
    char *ptr = buffer;
    while ((*ptr = getchar()) != '\n' && INDEX(ptr, buffer) < (size - 1)) {
        ++ptr;
    }
    *ptr = '\0';
}
