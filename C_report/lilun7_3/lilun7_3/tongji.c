
#include <stdio.h>

int main() {
    char str[101];
    int i;
    int upper = 0, lower = 0, digit = 0;

    printf("请输入一个字符串：\n");
    fgets(str, sizeof(str), stdin);

    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            upper++;
        }
        else if (str[i] >= 'a' && str[i] <= 'z') {
            lower++;
        }
        else if (str[i] >= '0' && str[i] <= '9') {
            digit++;
        }
    }

    printf("大写字母个数：%d\n", upper);
    printf("小写字母个数：%d\n", lower);
    printf("数字个数：%d\n", digit);

    return 0;
}
