#include "daiso_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     
#include <sys/socket.h> 
#include <netdb.h>      
#include <errno.h>

#define RECV_BUFFER_CHUNK_SIZE 4096

int daiso_net_connect(const char *host, const char *port) {
    struct addrinfo hints, *servinfo, *p;
    int sockfd = -1;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;   
    hints.ai_socktype = SOCK_STREAM; 

    printf("[Net] 正在解析: %s:%s\n", host, port);

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo 失败: %s\n", gai_strerror(rv));
        return -1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("[Net] socket 创建失败");
            continue; 
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("[Net] connect 失败");
            continue; 
        }

        printf("[Net] 连接成功 (fd=%d)\n", sockfd);
        break; 
    }

    freeaddrinfo(servinfo); 

    if (p == NULL) {
        fprintf(stderr, "[Net] 无法连接到 %s\n", host);
        return -1;
    }
    
    return sockfd;
}

int daiso_net_send(int sockfd, const char *data, size_t len) {
    return send(sockfd, data, len, 0);
}

int daiso_net_recv_dynamic(int sockfd, char **buffer_out) {
    char *buffer = NULL;
    size_t total_size = 0;
    size_t capacity = 0;
    int bytes_received;

    do {
        if (total_size + RECV_BUFFER_CHUNK_SIZE > capacity) {
            capacity += RECV_BUFFER_CHUNK_SIZE;
            char *new_buffer = realloc(buffer, capacity);
            if (!new_buffer) {
                fprintf(stderr, "[Net] realloc 内存分配失败\n");
                free(buffer);
                return -1;
            }
            buffer = new_buffer;
        }

        bytes_received = recv(sockfd, buffer + total_size, RECV_BUFFER_CHUNK_SIZE, 0);

        if (bytes_received > 0) {
            total_size += bytes_received;
        } else if (bytes_received == 0) {
            break;
        } else {
            perror("[Net] recv 失败");
            free(buffer);
            return -1;
        }

    } while (bytes_received > 0);
    
    if (total_size == capacity) {
         char *new_buffer = realloc(buffer, capacity + 1);
         if (!new_buffer) { 
             free(buffer);
             return -1;
         }
         buffer = new_buffer;
    }
    buffer[total_size] = '\0';
    
    *buffer_out = buffer;
    return total_size;
}
