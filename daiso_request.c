#include "daiso_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int daiso_request_build_get(const char *host, const char *path, char **request_buffer_out) {
    const char *template = "GET %s HTTP/1.0\r\n"
                           "Host: %s\r\n"
                           "User-Agent: libdaisohttp/0.1\r\n"
                           "Connection: close\r\n"
                           "\r\n";
    
    size_t len = snprintf(NULL, 0, template, path, host) + 1;

    char *buffer = malloc(len);
    if (!buffer) {
        perror("[Request] malloc 失败");
        *request_buffer_out = NULL;
        return -1;
    }

    snprintf(buffer, len, template, path, host);

    *request_buffer_out = buffer;
    return len - 1; 
}
