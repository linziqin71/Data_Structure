
#include <stdio.h>

#define N 10

// 输出当前代的细胞状态
void show(int a[N][N], int gen) {
    printf("Generation %d:\n", gen);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            putchar(a[i][j] ? '#' : '.');
        }
        putchar('\n');
    }
}

int main(void) {
    // 当前代和下一代
    int cur[N][N] = { 0 };
    int next[N][N] = { 0 };

    // 设置初始模式：Blinker 振荡器
    cur[4][3] = 1;
    cur[4][4] = 1;
    cur[4][5] = 1;

    // 输出初始状态
    show(cur, 0);

    // 连续演化10代
    for (int gen = 1; gen <= 10; gen++) {

        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {

                int n = 0;

                // 统计周围8个邻居中的活细胞数
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {

                        if (dr == 0 && dc == 0)
                            continue;

                        int nr = r + dr;
                        int nc = c + dc;

                        // 防止数组越界
                        if (nr >= 0 && nr < N &&
                            nc >= 0 && nc < N) {
                            n += cur[nr][nc];
                        }
                    }
                }

                // 根据生命游戏规则更新下一代
                next[r][c] =
                    (cur[r][c] && (n == 2 || n == 3)) ||
                    (!cur[r][c] && n == 3);
            }
        }

        // 将下一代复制到当前代
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                cur[r][c] = next[r][c];
            }
        }

        // 输出本代结果
        show(cur, gen);
    }

    return 0;
}
