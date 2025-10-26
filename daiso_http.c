#include "daiso_http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "daiso_net.h"
#include "daiso_request.h"
#include "daiso_response.h"

struct DaisoHttpHandle {
    char *host;
    char *path;
    char *port_str;
    int port_int;
};

static int _internal_parse_url(DaisoHttpHandle *h, const char *url) {
    char host_buf[256];
    char path_buf[1024];
    
    if (strncmp(url, "http://", 7) != 0) {
        fprintf(stderr, "[API] URL format error: only support http:// (Receive : %s)\n", url);
        return -1;
    }
    
    const char *url_no_scheme = url + 7;
    
    char *path_start = strchr(url_no_scheme, '/');
    if (!path_start) {
        h->host = strdup(url_no_scheme);
        h->path = strdup("/");
    } else {
        size_t host_len = path_start - url_no_scheme;
        h->host = malloc(host_len + 1);
        if (!h->host) return -1;
        strncpy(h->host, url_no_scheme, host_len);
        h->host[host_len] = '\0';
        
        h->path = strdup(path_start);
    }
    
    h->port_int = 80;
    h->port_str = strdup("80");
    
    if (!h->host || !h->path || !h->port_str) {
        free(h->host);
        free(h->path);
        free(h->port_str);
        return -1; 
    }
    
    printf("[API] URL Resolve: Host='%s', Path='%s', Port=%d\n", 
           h->host, h->path, h->port_int);
           
    return 0;
}


DaisoHttpHandle* daiso_http_init(void) {
    DaisoHttpHandle *handle = calloc(1, sizeof(DaisoHttpHandle));
    if (!handle) {
        perror("[API] daiso_http_init failed");
    }
    
    return handle;
}

int daiso_http_set_url(DaisoHttpHandle *handle, const char *url) {
    if (!handle || !url) return -1;
    
    free(handle->host);
    handle->host = NULL;
    free(handle->path);
    handle->path = NULL;
    free(handle->port_str);
    handle->port_str = NULL;
    
    return _internal_parse_url(handle, url);
}

DaisoHttpResponse* daiso_http_perform(DaisoHttpHandle *handle) {
    if (!handle || !handle->host || !handle->path) {
        fprintf(stderr, "[API] Perform failed: UNSET URL\n");
        return NULL;
    }
    
    int sockfd = -1;
    char *request_str = NULL;
    char *raw_response_str = NULL;
    DaisoHttpResponse *response = NULL;

    sockfd = daiso_net_connect(handle->host, handle->port_str);
    if (sockfd < 0) {
        goto cleanup; 
    }

    if (daiso_request_build_get(handle->host, handle->path, &request_str) < 0) {
        goto cleanup;
    }
    printf("\n[API] Sending request:\n%s", request_str);

    if (daiso_net_send(sockfd, request_str, strlen(request_str)) < 0) {
        perror("[API] Send request failed");
        goto cleanup;
    }

    if (daiso_net_recv_dynamic(sockfd, &raw_response_str) < 0) {
        goto cleanup;
    }
    printf("\n[API] Received origin datas (All %zu bytes)\n", raw_response_str ? strlen(raw_response_str) : 0);

    response = daiso_response_parse(raw_response_str);
    if (!response) {
        goto cleanup;
    }
    
    raw_response_str = NULL; 

cleanup:
    if (sockfd >= 0) close(sockfd);
    free(request_str); 
    
    free(raw_response_str); 
    
    return response;
}

void daiso_http_response_free(DaisoHttpResponse *response) {
    if (!response) return;
    
    free(response->header); 
    
    free(response->status_text);
    
    free(response);
}

void daiso_http_cleanup(DaisoHttpHandle *handle) {
    if (!handle) return;
    
    free(handle->host);
    free(handle->path);
    free(handle->port_str);
    
    free(handle);
}
