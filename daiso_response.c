#include "daiso_response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char* stristr(const char* haystack, const char* needle) {
    if (!needle || !*needle) return (char*)haystack;
    for (; *haystack; ++haystack) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h, *n;
            for (h = haystack, n = needle; *h && *n; ++h, ++n) {
                if (tolower((unsigned char)*h) != tolower((unsigned char)*n)) {
                    break;
                }
            }
            if (!*n) { 
                return (char*)haystack;
            }
        }
    }
    return NULL;
}


DaisoHttpResponse* daiso_response_parse(char *raw_response) {
    if (!raw_response) return NULL;

    DaisoHttpResponse *resp = calloc(1, sizeof(DaisoHttpResponse));
    if (!resp) {
        perror("[Response] calloc failed");
        return NULL;
    }

    char *separator = strstr(raw_response, "\r\n\r\n");
    if (!separator) {
        fprintf(stderr, "[Response] Cant find HTTP head separator\n");
        resp->status_code = -1; 
        resp->body = raw_response; 
        resp->body_len = strlen(raw_response);
        return resp; 
    }

    *separator = '\0';
    resp->header = raw_response;
    
    resp->body = separator + 4;
    resp->body_len = strlen(resp->body); 

    char http_ver[32];
    char status_text_buf[256];
    
    if (sscanf(resp->header, "%31s %d %255[^\r\n]", 
               http_ver, 
               &resp->status_code, 
               status_text_buf) == 3) 
    {
        resp->status_text = strdup(status_text_buf); 
    } else {
        fprintf(stderr, "[Response] No Resolve mode running! \n");
        resp->status_code = -1; 
    }

    char *content_len_str = stristr(resp->header, "Content-Length:");
    if (content_len_str) {
    }

    return resp;
}
