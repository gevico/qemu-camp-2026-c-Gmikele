#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    const char *start = cmd + 2; // 跳过"s/"
    const char *first_slash = strchr(start, '/');
    if (!first_slash) {
        return -1;
    }
    
    const char *second_slash = strchr(first_slash + 1, '/');
    if (!second_slash) {
        return -1;
    }
    
    // 提取旧字符串
    int old_len = first_slash - start;
    *old_str = (char*)malloc(old_len + 1);
    if (!*old_str) {
        return -1;
    }
    strncpy(*old_str, start, old_len);
    (*old_str)[old_len] = '\0';
    
    // 提取新字符串
    int new_len = second_slash - (first_slash + 1);
    *new_str = (char*)malloc(new_len + 1);
    if (!*new_str) {
        free(*old_str);
        return -1;
    }
    strncpy(*new_str, first_slash + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    char *pos = strstr(str, old);
    if (pos == NULL) {
        return; // 没有找到匹配
    }
    
    int old_len = strlen(old);
    int new_len = strlen(new);
    
    if (new_len > old_len) {
        // 新字符串更长，需要移动后面的字符
        int diff = new_len - old_len;
        int remaining_len = strlen(pos + old_len);
        
        // 移动后面的字符
        memmove(pos + new_len, pos + old_len, remaining_len + 1);
    } else if (new_len < old_len) {
        // 新字符串更短，需要向前移动后面的字符
        int diff = old_len - new_len;
        int remaining_len = strlen(pos + old_len);
        
        // 移动后面的字符
        memmove(pos + new_len, pos + old_len, remaining_len + 1);
    }
    
    // 复制新字符串
    memcpy(pos, new, new_len);
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
