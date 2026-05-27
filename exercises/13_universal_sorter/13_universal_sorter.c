#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*CompareFunc)(const void *, const void *);

int compareInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compareFloat(const void *a, const void *b) {
    float diff = (*(float*)a - *(float*)b);
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

int compareString(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

void sort(void *array, size_t n, size_t size, CompareFunc compare) {
    qsort(array, n, size, compare);
}

void processFile(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("错误: 无法打开文件 %s\n", filename);
        return;
    }

    int choice, n;
    if (fscanf(fin, "%d", &choice) != 1 || fscanf(fin, "%d", &n) != 1) {
        printf("错误: 文件 %s 格式不正确\n", filename);
        fclose(fin);
        return;
    }

    if (n > 20) n = 20;  // 最多支持20个元素

    printf("=== 处理数据来自: %s ===\n", filename);

    switch (choice) {
        case 1: { // 整数排序
            int arr[20];
            for (int i = 0; i < n; i++) {
                if (fscanf(fin, "%d", &arr[i]) != 1) {
                    printf("错误: 读取整数失败\n");
                    return;
                }
            }
            
            printf("原始整数数组: ");
            for (int i = 0; i < n; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
            
            sort(arr, n, sizeof(int), compareInt);
            
            printf("排序后整数数组: ");
            for (int i = 0; i < n; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
            break;
        }
        
        case 2: { // 浮点数排序
            float arr[20];
            for (int i = 0; i < n; i++) {
                if (fscanf(fin, "%f", &arr[i]) != 1) {
                    printf("错误: 读取浮点数失败\n");
                    return;
                }
            }
            
            printf("原始浮点数数组: ");
            for (int i = 0; i < n; i++) {
                printf("%.2f ", arr[i]);
            }
            printf("\n");
            
            sort(arr, n, sizeof(float), compareFloat);
            
            printf("排序后浮点数数组: ");
            for (int i = 0; i < n; i++) {
                printf("%.2f ", arr[i]);
            }
            printf("\n");
            break;
        }
        
        case 3: { // 字符串排序
            char *arr[20];
            char buffer[100];
            
            for (int i = 0; i < n; i++) {
                if (fscanf(fin, "%s", buffer) != 1) {
                    printf("错误: 读取字符串失败\n");
                    return;
                }
                arr[i] = strdup(buffer);
                if (!arr[i]) {
                    printf("错误: 内存分配失败\n");
                    return;
                }
            }
            
            printf("原始字符串数组: ");
            for (int i = 0; i < n; i++) {
                printf("%s ", arr[i]);
            }
            printf("\n");
            
            sort(arr, n, sizeof(char*), compareString);
            
            printf("排序后字符串数组: ");
            for (int i = 0; i < n; i++) {
                printf("%s ", arr[i]);
            }
            printf("\n");
            
            // 释放内存
            for (int i = 0; i < n; i++) {
                free(arr[i]);
            }
            break;
        }
        
        default:
            printf("错误: 未知的数据类型选择 %d\n", choice);
            break;
    }

    fclose(fin);
}

int main() {
    processFile("int_sort.txt");
    processFile("float_sort.txt");

    return 0;
}