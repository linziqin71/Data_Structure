
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX_STUDENTS 100

/* 定义统计结果结构体 */
typedef struct
{
    double average;
    double passRate;
    int highest;
    int lowest;
    int passed;
} Stats;

/* 函数声明 */
int isValidScore(int score);
char getGrade(int score);
Stats statistics(int scores[], int n);
void diagnose(Stats s);
void showResult(Stats s, int n);

int main(void)
{
    int scores[MAX_STUDENTS];
    int count = 0;
    int score, ch;
    Stats result;

    printf("===== 学生成绩质量诊断器 =====\n");
    printf("请输入0~100的成绩，输入-1结束。\n");

    while (count < MAX_STUDENTS)
    {
        printf("请输入第%d名学生的成绩：", count + 1);

        /* 检查是否成功输入整数 */
        if (scanf("%d", &score) != 1)
        {
            if (feof(stdin))
                break;

            printf("输入错误，请输入整数！\n");

            while ((ch = getchar()) != '\n' && ch != EOF)
            {
            }

            if (ch == EOF)
                break;

            continue;
        }

        /* 清理当前输入行多余字符 */
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
        }

        /* 判断是否结束输入 */
        if (score == -1)
            break;

        /* 调用函数检查成绩是否合法 */
        if (!isValidScore(score))
        {
            printf("成绩无效！请输入0~100的整数。\n");
            continue;
        }

        /* 保存有效成绩 */
        scores[count] = score;
        count++;

        /* 调用函数获取成绩等级 */
        printf("录入成功，等级为：%c\n", getGrade(score));
    }

    /* 判断是否有有效数据 */
    if (count == 0)
    {
        printf("没有有效成绩，无法进行统计。\n");
        return 0;
    }

    /* 调用统计函数 */
    result = statistics(scores, count);

    /* 输出统计结果 */
    showResult(result, count);

    /* 输出整体成绩诊断 */
    diagnose(result);

    return 0;
}

/* 函数1：检查成绩是否合法 */
int isValidScore(int score)
{
    if (score >= 0 && score <= 100)
        return 1;
    else
        return 0;
}

/* 函数2：将成绩转换为等级 */
char getGrade(int score)
{
    if (score >= 90)
        return 'A';
    else if (score >= 80)
        return 'B';
    else if (score >= 70)
        return 'C';
    else if (score >= 60)
        return 'D';
    else
        return 'F';
}

/* 函数3：统计成绩 */
Stats statistics(int scores[], int n)
{
    Stats s;
    double sum = 0;
    int i;

    s.highest = scores[0];
    s.lowest = scores[0];
    s.passed = 0;

    for (i = 0; i < n; i++)
    {
        sum += scores[i];

        if (scores[i] > s.highest)
            s.highest = scores[i];

        if (scores[i] < s.lowest)
            s.lowest = scores[i];

        if (scores[i] >= 60)
            s.passed++;
    }

    s.average = sum / n;
    s.passRate = 100.0 * s.passed / n;

    return s;
}

/* 函数4：输出成绩诊断 */
void diagnose(Stats s)
{
    printf("\n===== 成绩质量诊断 =====\n");

    if (s.average >= 85 && s.passRate >= 90)
    {
        printf("整体成绩优秀，学习效果较好。\n");
    }
    else if (s.average >= 70 && s.passRate >= 80)
    {
        printf("整体成绩良好，建议继续巩固知识。\n");
    }
    else if (s.passRate >= 60)
    {
        printf("整体成绩一般，需要加强薄弱知识点。\n");
    }
    else
    {
        printf("整体成绩有待提高，建议加强基础练习。\n");
    }

    if (s.highest - s.lowest >= 40)
    {
        printf("最高分与最低分差距较大，"
            "应关注学生之间的学习差异。\n");
    }
}

/* 函数5：输出统计结果 */
void showResult(Stats s, int n)
{
    printf("\n===== 成绩统计结果 =====\n");
    printf("有效人数：%d\n", n);
    printf("平均分：%.2f\n", s.average);
    printf("最高分：%d\n", s.highest);
    printf("最低分：%d\n", s.lowest);
    printf("及格人数：%d\n", s.passed);
    printf("及格率：%.2f%%\n", s.passRate);
}
