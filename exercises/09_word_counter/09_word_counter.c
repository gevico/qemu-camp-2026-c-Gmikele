#include <stdio.h>

#include <ctype.h>

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    int inWord = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i]) || str[i] == '\'') {
            // 字母或撇号（如Don't）属于单词
            if (!inWord) {
                inWord = 1;
                wordCount++;
            }
        } else {
            // 非字母字符，单词结束
            inWord = 0;
        }
    }
    
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}
