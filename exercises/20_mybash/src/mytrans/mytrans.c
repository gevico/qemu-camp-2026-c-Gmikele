// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    if (!str || !*str) {
        return;
    }
    
    // 去除开头的空白字符
    char *start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }
    
    // 如果整个字符串都是空白
    if (!*start) {
        *str = '\0';
        return;
    }
    
    // 去除结尾的空白字符
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    
    // 移动字符串到开头
    if (start != str) {
        memmove(str, start, end - start + 1);
    }
    
    // 添加终止符
    str[end - start + 1] = '\0';
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
  int expect_translation = 0; // 0: 期望单词行, 1: 期望翻译行

  *dict_count = 0;
  
  while (fgets(line, sizeof(line), file) != NULL) {
    trim(line);
    
    if (strlen(line) == 0) {
      continue;
    }
    
    // 检查是否是单词行（以#开头）
    if (line[0] == '#') {
      // 如果已经有正在处理的词条，先保存它
      if (in_entry && strlen(current_word) > 0 && strlen(current_translation) > 0) {
        hash_table_insert(table, current_word, current_translation);
        (*dict_count)++;
        
        // 重置缓冲区
        current_word[0] = '\0';
        current_translation[0] = '\0';
      }
      
      // 开始新的词条
      in_entry = 1;
      expect_translation = 1;
      
      // 提取单词（跳过开头的#字符）
      char *word_start = line + 1;
      // 单词是整行（去掉#后）
      strncpy(current_word, word_start, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      
      // 清除当前翻译，等待翻译行
      current_translation[0] = '\0';
    } 
    // 检查是否是翻译行（以"Trans:"开头）
    else if (in_entry && expect_translation && strncmp(line, "Trans:", 6) == 0) {
      // 提取翻译（跳过"Trans:"前缀）
      char *translation_start = line + 6;
      // 跳过可能的前导空格
      while (*translation_start == ' ') {
        translation_start++;
      }
      
      if (strlen(translation_start) > 0) {
        strncpy(current_translation, translation_start, sizeof(current_translation) - 1);
        current_translation[sizeof(current_translation) - 1] = '\0';
      }
      expect_translation = 0;
    }
    else if (in_entry && !expect_translation) {
      // 这是翻译的续行，追加到当前翻译
      if (strlen(current_translation) + strlen(line) + 2 < sizeof(current_translation)) {
        if (strlen(current_translation) > 0) {
          strcat(current_translation, " ");
        }
        strcat(current_translation, line);
      }
    }
    // 其他情况忽略
  }
  
  // 处理最后一个词条
  if (in_entry && strlen(current_word) > 0 && strlen(current_translation) > 0) {
    hash_table_insert(table, current_word, current_translation);
    (*dict_count)++;
  }

  fclose(file);
  return 0;
}

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char* filename) {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  // 构建正确的dict.txt路径
  char dict_path[1024];
  snprintf(dict_path, sizeof(dict_path), "%s/src/mytrans/dict.txt", 
           filename ? strstr(filename, "20_mybash") ? "" : "/home/ruilun_yang/QEMU_C/qemu-camp-2026-c-Gmikele/exercises/20_mybash" : "/home/ruilun_yang/QEMU_C/qemu-camp-2026-c-Gmikele/exercises/20_mybash");
  
  // 尝试多个可能的路径
  const char* possible_paths[] = {
    "/home/ruilun_yang/QEMU_C/qemu-camp-2026-c-Gmikele/exercises/20_mybash/src/mytrans/dict.txt",
    "./src/mytrans/dict.txt",
    "src/mytrans/dict.txt",
    NULL
  };
  
  int loaded = 0;
  for (int i = 0; possible_paths[i] != NULL; i++) {
    if (load_dictionary(possible_paths[i], table, &dict_count) == 0) {
      loaded = 1;
      break;
    }
  }
  
  if (!loaded) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 dict.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }

    // 使用 strtok 按空格分割单词
    char *word = strtok(line, " ");
    while (word != NULL) {
      // 将单词转换为小写进行查找（字典中的单词是小写）
      char lower_word[256];
      strncpy(lower_word, word, sizeof(lower_word) - 1);
      lower_word[sizeof(lower_word) - 1] = '\0';
      to_lowercase(lower_word);
      
      const char *translation = hash_table_lookup(table, lower_word);
      printf("原文: %s\t", word);
      if (translation) {
          printf("翻译: %s\n", translation);
      } else {
          printf("未找到该单词的翻译。\n");
      }

      word = strtok(NULL, " ");
    }
  }

  free_hash_table(table);
  return 0;
}