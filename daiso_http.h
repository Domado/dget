#ifndef DAISO_HTTP_H
#define DAISO_HTTP_H

#include <stddef.h> 

typedef struct {
    char *header;      
    char *body;        
    size_t body_len;   
    int status_code;   
    char *status_text; 
} DaisoHttpResponse;

typedef struct DaisoHttpHandle DaisoHttpHandle;

DaisoHttpHandle* daiso_http_init(void);

int daiso_http_set_url(DaisoHttpHandle *handle, const char *url);

DaisoHttpResponse* daiso_http_perform(DaisoHttpHandle *handle);

void daiso_http_response_free(DaisoHttpResponse *response);

void daiso_http_cleanup(DaisoHttpHandle *handle);


#endif
