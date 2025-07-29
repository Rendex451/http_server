#ifndef NET_H
#define NET_H

#include <stddef.h>

int net_listen(char *address);
int net_accept(int listener);

int net_connect(char *address);
int net_close(int conn);

int net_send(int conn, char *buffer, size_t size);
int net_recv(int conn, char *buffer, size_t size);

#endif /* NET_H */
