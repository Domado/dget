#ifndef DAISO_REQUEST_H
#define DAISO_REQUEST_H

#include <stddef.h> 

int daiso_request_build_get(const char *host, const char *path, char **request_buffer_out);

#endif
