// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    char *end;
    
    // 去除前导空格
    while (isspace((unsigned char)*str)) {
        str++;
    }
    
    // 如果字符串全是空格
    if (*str == 0) {
        return;
    }
    
    // 去除尾随空格
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    
    // 写入新的终止符
    *(end + 1) = '\0';
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    trim(line);
    
    if (strlen(line) == 0) {
      continue;
    }
    
    // 检查是否是单词行（以"#"开头）
    if (line[0] == '#') {
      if (in_entry && strlen(current_word) > 0 && strlen(current_translation) > 0) {
        // 保存前一个词条
        hash_table_insert(table, current_word, current_translation);
        (*dict_count)++;
      }
      
      // 开始新词条
      strcpy(current_word, line + 1); // 跳过"#"
      current_translation[0] = '\0';
      in_entry = 1;
    } else if (in_entry && strncmp(line, "Trans:", 6) == 0) {
      // 翻译行，跳过"Trans:"前缀
      const char *translation = line + 6;
      if (strlen(current_translation) > 0) {
        strcat(current_translation, "@");
      }
      strcat(current_translation, translation);
    }
  }
  
  // 处理最后一个词条
  if (in_entry && strlen(current_word) > 0 && strlen(current_translation) > 0) {
    hash_table_insert(table, current_word, current_translation);
    (*dict_count)++;
  }

  fclose(file);
  return 0;
}
