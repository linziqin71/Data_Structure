
#include <stdio.h>

void reverse(int arr[], int n) {
    int i, temp;

    for (i = 0; i < n / 2; i++) {
        temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}

int main() {
    int a[5] = { 1, 2, 3, 4, 5 };
    int i;

    printf("ÄæÐòÇ°£º");
    for (i = 0; i < 5; i++) {
        printf("%d ", a[i]);
    }

    reverse(a, 5);

    printf("\nÄæÐòºó£º");
    for (i = 0; i < 5; i++) {
        printf("%d ", a[i]);
    }

    printf("\n");

    return 0;
}
