
#include <stdio.h>
#include <limits.h>
#include "calc.h"

int main(void)
{
    int a, b, choice;
    long long check;

    printf("===== 简易计算器 =====\n");
    printf("1. 加法\n");
    printf("2. 减法\n");
    printf("3. 乘法\n");
    printf("4. 除法\n");

    printf("请选择运算(1~4)：");

    if (scanf_s("%d", &choice) != 1 ||
        choice < 1 || choice > 4)
    {
        printf("运算选择错误！\n");
        return 1;
    }

    printf("请输入两个整数：");

    if (scanf_s("%d %d", &a, &b) != 2)
    {
        printf("输入错误！\n");
        return 1;
    }

    switch (choice)
    {
    case 1:
        /* 加法溢出检查 */
        check = (long long)a + b;

        if (check > INT_MAX || check < INT_MIN)
        {
            printf("计算结果超出int范围！\n");
            return 1;
        }

        printf("结果：%d\n", add(a, b));
        break;

    case 2:
        /* 减法溢出检查 */
        check = (long long)a - b;

        if (check > INT_MAX || check < INT_MIN)
        {
            printf("计算结果超出int范围！\n");
            return 1;
        }

        printf("结果：%d\n", subtract(a, b));
        break;

    case 3:
        /* 乘法溢出检查 */
        check = (long long)a * b;

        if (check > INT_MAX || check < INT_MIN)
        {
            printf("计算结果超出int范围！\n");
            return 1;
        }

        printf("结果：%d\n", multiply(a, b));
        break;

    case 4:
        /* 防止除数为零 */
        if (b == 0)
        {
            printf("错误：除数不能为0！\n");
            return 1;
        }

        printf("结果：%.2f\n", divide(a, b));
        break;
    }

    return 0;
}
