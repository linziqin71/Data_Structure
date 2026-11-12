
#include <stdio.h>

/* 递归函数：计算n的阶乘 */
int factorial(int n)
{
    /* 递归终止条件 */
    if (n == 1)
        return 1;

    /* 递归调用 */
    return n * factorial(n - 1);
}

int main(void)
{
    int n, result;

    /* 输入正整数 */
    printf("请输入正整数n(1~12)：");

    if (scanf_s("%d", &n) != 1)
    {
        printf("输入错误！\n");
        return 1;
    }

    /* 判断输入范围 */
    if (n < 1 || n > 12)
    {
        printf("请输入1~12之间的整数！\n");
        return 1;
    }

    /* 调用递归函数 */
    result = factorial(n);

    /* 输出计算结果 */
    printf("%d! = %d\n", n, result);

    return 0;
}
