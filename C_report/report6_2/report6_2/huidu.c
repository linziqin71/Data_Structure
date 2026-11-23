#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define N 10
#define TH 2

int main(void) {
    int a[N][N];
    int basic, improved;

    // 上、下、左、右四个方向
    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };

    // 输入灰度矩阵
    printf("Input 10x10 grayscale matrix:\n");
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (scanf("%d", &a[r][c]) != 1 ||
                a[r][c] < 0 || a[r][c] > 9)
                return 1;
        }
    }

    // 输出原始矩阵
    puts("Original:");
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            printf("%d", a[r][c]);
        }
        putchar('\n');
    }

    // 基础版：只比较右侧和下方
    puts("Right/down:");
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            basic =
                (c + 1 < N &&
                    abs(a[r][c] - a[r][c + 1]) > TH) ||
                (r + 1 < N &&
                    abs(a[r][c] - a[r + 1][c]) > TH);

            putchar(basic ? '#' : '.');
        }
        putchar('\n');
    }

    // 改进版：比较上下左右四个方向
    puts("Four directions:");
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            improved = 0;

            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k];
                int nc = c + dc[k];

                // 判断相邻位置是否越界
                if (nr >= 0 && nr < N &&
                    nc >= 0 && nc < N &&
                    abs(a[r][c] - a[nr][nc]) > TH) {
                    improved = 1;
                }
            }

            putchar(improved ? '#' : '.');
        }
        putchar('\n');
    }

    return 0;
}
