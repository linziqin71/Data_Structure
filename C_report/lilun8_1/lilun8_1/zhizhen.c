
#include <stdio.h>

// 使用指针计算数组元素之和
int sum_array(int* p, int n)
{
    int sum = 0;

    for (int i = 0; i < n; i++)
    {
        sum += *(p + i);
    }

    return sum;
}

int main()
{
    int arr[5] = { 1, 2, 3, 4, 5 };

    int result = sum_array(arr, 5);

    printf("数组元素之和为：%d\n", result);

    return 0;
}
