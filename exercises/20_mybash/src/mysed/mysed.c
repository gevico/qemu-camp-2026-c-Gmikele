#include "mysed.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }
    const char *start = cmd + 2;
    const char *first_slash = strchr(start, '/');
    if (!first_slash) return -1;
    const char *second_slash = strchr(first_slash + 1, '/');
    if (!second_slash) return -1;

    int old_len = first_slash - start;
    *old_str = (char*)malloc(old_len + 1);
    if (!*old_str) return -1;
    strncpy(*old_str, start, old_len);
    (*old_str)[old_len] = '\0';

    int new_len = second_slash - (first_slash + 1);
    *new_str = (char*)malloc(new_len + 1);
    if (!*new_str) { free(*old_str); return -1; }
    strncpy(*new_str, first_slash + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    char *pos = strstr(str, old);
    if (!pos) return;
    int old_len = strlen(old);
    int new_len = strlen(new);
    if (new_len != old_len)
        memmove(pos + new_len, pos + old_len, strlen(pos + old_len) + 1);
    memcpy(pos, new, new_len);
}

int __cmd_mysed(const char *old_str_input, const char *new_str_input) {
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        replace_first_occurrence(line, old_str_input, new_str_input);
        fputs(line, stdout);
    }
    return 0;
}