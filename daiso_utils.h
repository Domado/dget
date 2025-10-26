#ifndef DAISO_UTILS_H
#define DAISO_UTILS_H

typedef struct {
    char *scheme;   
    char *host;     
    char *path;     
    int port;       
} DaisoParsedUrl;

DaisoParsedUrl* daiso_utils_parse_url(const char *raw_url);

void daiso_utils_free_url(DaisoParsedUrl *url);


#endif
