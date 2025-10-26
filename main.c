#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "daiso_http.h" 

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "用法: %s <url>\n", argv[0]);
        fprintf(stderr, "示例: %s http://example.com/\n", argv[0]);
        return 1;
    }

    const char *url = argv[1];
    DaisoHttpHandle *http = NULL;
    DaisoHttpResponse *resp = NULL;

    printf("--- Daiso HTTP Client 测试 ---\n");

    http = daiso_http_init();
    if (!http) {
        fprintf(stderr, "初始化失败\n");
        return 1;
    }

    if (daiso_http_set_url(http, url) != 0) {
        fprintf(stderr, "设置 URL 失败: %s\n", url);
        goto cleanup;
    }

    resp = daiso_http_perform(http);
    if (!resp) {
        fprintf(stderr, "执行请求失败\n");
        goto cleanup;
    }

    printf("\n--- 请求结果 ---\n");
    printf("状态: %d %s\n", resp->status_code, resp->status_text ? resp->status_text : "");
    
    printf("--- 响应头 (部分) ---\n");
    if(resp->header) {
        char *header_copy = strdup(resp->header); 
        if(header_copy) {
            char *line = strtok(header_copy, "\r\n");
            for(int i=0; line && i < 5; i++) {
                printf("%s\n", line);
                line = strtok(NULL, "\r\n");
            }
            free(header_copy);
        }
    }
    
    printf("\n--- 响应体 (前 200 字节) ---\n");
    if(resp->body) {
        printf("%.*s\n", 200, resp->body);
        printf("...\n(总 body 长度: %zu 字节)\n", resp->body_len);
    }


cleanup:
    printf("\n--- 清理资源 ---\n");
    daiso_http_response_free(resp); 
    daiso_http_cleanup(http);       

    return 0;
}
