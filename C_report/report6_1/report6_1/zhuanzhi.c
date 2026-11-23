#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>

int main(void) {
    int a[3][4], t[4][3];
    char s[256];
    int letters = 0, digits = 0;
    int spaces = 0, others = 0;

    puts("Input 12 integers:");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (scanf("%d", &a[i][j]) != 1)
                return 1;
        }
    }

    // 矩阵转置
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            t[j][i] = a[i][j];
        }
    }

    puts("Transpose (4x3):");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d%c", t[i][j],
                j == 2 ? '\n' : ' ');
        }
    }

    // 清除输入缓冲区中的换行符
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    puts("Input a line:");
    if (!fgets(s, sizeof(s), stdin))
        return 1;

    // 统计各类字符
    for (int i = 0; s[i] != '\0' && s[i] != '\n'; i++) {
        unsigned char c = (unsigned char)s[i];

        if (isalpha(c))
            letters++;
        else if (isdigit(c))
            digits++;
        else if (c == ' ')
            spaces++;
        else
            others++;
    }

    printf("Letters=%d Digits=%d Spaces=%d Others=%d\n",
        letters, digits, spaces, others);

    return 0;
}
