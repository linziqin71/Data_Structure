
#include <stdio.h>

/* 函数声明 */
double getArea(double r);

int main(void)
{
    double r, area;

    /* 输入圆的半径 */
    printf("请输入圆的半径：");
    if (scanf_s("%lf", &r) != 1 || !(r >= 0))
    {
        printf("输入错误，半径不能为负数！\n");
        return 1;
    }

    /* 调用函数计算圆的面积 */
    area = getArea(r);

    /* 输出结果 */
    printf("圆的面积为：%.2f\n", area);

    return 0;
}

/* 函数定义放在main函数之后 */
double getArea(double r)
{
    return 3.14 * r * r;
}
