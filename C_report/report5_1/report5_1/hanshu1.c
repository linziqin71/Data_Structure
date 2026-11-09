
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/* 函数声明 */
int gcd(int a, int b);
int isPrime(int n);
int max3(int a, int b, int c);
double average(int a[], int n);

int main(void)
{
    int choice, a, b, c;
    int nums[100], n, i;

    while (1)
    {
        printf("\n===== 数学函数工具箱 =====\n");
        printf("1. 求最大公约数\n");
        printf("2. 判断素数\n");
        printf("3. 求三个数的最大值\n");
        printf("4. 求若干数的平均值\n");
        printf("0. 退出程序\n");
        printf("请选择：");

        if (scanf("%d", &choice) != 1)
        {
            printf("输入错误！\n");
            break;
        }

        if (choice == 0)
            break;

        switch (choice)
        {
        case 1:
            printf("请输入两个正整数：");
            if (scanf("%d %d", &a, &b) != 2 ||
                a <= 0 || b <= 0)
            {
                printf("输入无效！\n");
                return 1;
            }

            printf("最大公约数：%d\n", gcd(a, b));
            break;

        case 2:
            printf("请输入一个整数：");
            if (scanf("%d", &a) != 1)
                return 1;

            if (isPrime(a))
                printf("%d 是素数。\n", a);
            else
                printf("%d 不是素数。\n", a);
            break;

        case 3:
            printf("请输入三个整数：");
            if (scanf("%d %d %d", &a, &b, &c) != 3)
                return 1;

            printf("最大值：%d\n", max3(a, b, c));
            break;

        case 4:
            printf("请输入整数个数(1~100)：");
            if (scanf("%d", &n) != 1 ||
                n < 1 || n > 100)
            {
                printf("个数无效！\n");
                return 1;
            }

            for (i = 0; i < n; i++)
            {
                printf("第%d个整数：", i + 1);
                if (scanf("%d", &nums[i]) != 1)
                    return 1;
            }

            printf("平均值：%.2f\n", average(nums, n));
            break;

        default:
            printf("无效菜单选项！\n");
        }
    }

    printf("程序结束。\n");
    return 0;
}

/* 最大公约数：欧几里得算法 */
int gcd(int a, int b)
{
    int r;

    while (b != 0)
    {
        r = a % b;
        a = b;
        b = r;
    }

    return a;
}

/* 素数判断：1表示素数，0表示非素数 */
int isPrime(int n)
{
    int i;

    if (n < 2)
        return 0;

    for (i = 2; i <= n / i; i++)
    {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

/* 求三个整数中的最大值 */
int max3(int a, int b, int c)
{
    int max = a;

    if (b > max)
        max = b;

    if (c > max)
        max = c;

    return max;
}

/* 求数组中n个整数的平均值 */
double average(int a[], int n)
{
    double sum = 0;
    int i;

    for (i = 0; i < n; i++)
        sum += a[i];

    return sum / n;
}
