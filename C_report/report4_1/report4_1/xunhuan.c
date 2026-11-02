
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
    int score, count = 0, grades[5] = { 0 };
    int maxScore = 0, minScore = 0, ch, i;
    long long sum = 0;
    const char names[] = "ABCDF";

    printf("=== 菜单式成绩分析程序 ===\n");
    printf("连续输入成绩（0~100），输入 -1 结束。\n");

    while (1)
    {
        printf("请输入成绩：");

        if (scanf("%d", &score) != 1)
        {
            if (feof(stdin)) break;

            printf("输入错误：请输入整数。\n");

            while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        /* 清理当前输入行的剩余字符 */
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        /* 判断结束标志 */
        if (score == -1)
            break;

        /* 判断成绩是否合法 */
        if (score < 0 || score > 100)
        {
            printf("成绩必须在0~100之间，请重新输入。\n");
            continue;
        }

        /* 更新最高分和最低分 */
        if (count == 0)
            maxScore = minScore = score;

        if (score > maxScore) maxScore = score;
        if (score < minScore) minScore = score;

        /* 统计总分和人数 */
        sum += score;
        count++;

        /* 按分数划分等级 */
        if (score >= 90) grades[0]++;
        else if (score >= 80) grades[1]++;
        else if (score >= 70) grades[2]++;
        else if (score >= 60) grades[3]++;
        else grades[4]++;
    }

    /* 避免没有成绩时除以零 */
    if (count == 0)
    {
        printf("没有有效成绩，无法计算平均分与等级占比。\n");
        return 0;
    }

    /* 输出统计结果 */
    printf("\n有效人数：%d\n", count);
    printf("平均分：%.2f\n", (double)sum / count);
    printf("最高分：%d\n", maxScore);
    printf("最低分：%d\n", minScore);

    /* 输出各等级的人数和占比 */
    for (i = 0; i < 5; i++)
    {
        printf("%c级：%d人，占比：%.2f%%\n",
            names[i], grades[i],
            100.0 * grades[i] / count);
    }

    return 0;
}
