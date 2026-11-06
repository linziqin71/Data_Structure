
#include <stdio.h>

/* 定义函数，返回两个整数中的较大值 */
int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int main(void)
{
    int x, y, result;

    /* 输入两个整数 */
    printf("请输入两个整数：");
    if (scanf_s("%d %d", &x, &y) != 2)
    {
        printf("输入错误！\n");
        return 1;
    }

    /* 调用max函数 */
    result = max(x, y);

    /* 输出结果 */
    printf("较大的整数是：%d\n", result);

    return 0;
}
