#ifndef DAISO_NET_H
#define DAISO_NET_H

#include <stddef.h> 

int daiso_net_connect(const char *host, const char *port);

int daiso_net_send(int sockfd, const char *data, size_t len);

int daiso_net_recv_dynamic(int sockfd, char **buffer_out);

#endif
