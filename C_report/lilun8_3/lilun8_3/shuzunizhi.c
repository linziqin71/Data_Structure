
#include <stdio.h>

void reverse(int* arr, int n)
{
    int* left = arr;
    int* right = arr + n - 1;

    while (left < right)
    {
        int temp = *left;
        *left = *right;
        *right = temp;

        left++;
        right--;
    }
}

int main()
{
    int arr[5] = { 1, 2, 3, 4, 5 };

    printf("逆序前的数组：");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", arr[i]);
    }

    reverse(arr, 5);

    printf("\n逆序后的数组：");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    return 0;
}
