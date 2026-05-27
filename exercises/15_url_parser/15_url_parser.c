#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * URL解码函数：仅将%XX编码转换为字符，+保持不变
 */
static void url_decode(char *str) {
    char *src = str;
    char *dst = str;
    while (*src) {
        if (*src == '%' && isxdigit((unsigned char)src[1]) && isxdigit((unsigned char)src[2])) {
            char hex[3] = {src[1], src[2], '\0'};
            *dst++ = (char)strtol(hex, NULL, 16);
            src += 3;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

int parse_url(const char* url) {
    const char *query_start = strchr(url, '?');
    if (!query_start) return 0;
    query_start++;
    
    const char *fragment_start = strchr(query_start, '#');
    size_t query_len = fragment_start ? (size_t)(fragment_start - query_start) : strlen(query_start);
    
    char *query = malloc(query_len + 1);
    if (!query) return -1;
    strncpy(query, query_start, query_len);
    query[query_len] = '\0';
    
    char *token = strtok(query, "&");
    int param_count = 0;
    
    while (token) {
        char *equal_sign = strchr(token, '=');
        if (equal_sign) {
            *equal_sign = '\0';
            char *key = token;
            char *value = equal_sign + 1;
            url_decode(key);
            url_decode(value);
            printf("key = %s, value = %s\n", key, value);
            param_count++;
        } else if (token[0] != '\0') {
            url_decode(token);
            printf("%s=\n", token);
            param_count++;
        }
        token = strtok(NULL, "&");
    }
    
    free(query);
    return param_count;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York&email=user%40example.com";
    parse_url(test_url);
    return 0;
}