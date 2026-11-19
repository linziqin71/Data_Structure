#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    int a[3][4];
    int b[4][3];
    int i, j;

    printf("请输入3行4列矩阵的12个元素：\n");

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            b[j][i] = a[i][j];
        }
    }

    printf("转置后的矩阵为：\n");

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            printf("%4d", b[i][j]);
        }
        printf("\n");
    }

    return 0;
}
