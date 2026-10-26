
#define _CRT_SECURE_NO_WARNINGS   //注意得放在开头
#include <stdio.h>

#define FILENAME "score.txt"     
#define STUDENT_COUNT 3          //定义两个宏，方便替换

int main(void)
{
    FILE* fp;
    char id[20], name[30];
    float s1, s2, s3, average;
    int i, count, ch;    //i是第几名学生，count读取的时候用，ch清理剩余的字符

    printf("========== 学生成绩记录器 ==========\n\n");

    //第一部分：读取已有的学生记录 
    fp = fopen(FILENAME, "r");

    if (fp == NULL)
    {
        printf("暂无历史记录，首次运行将创建文件。\n\n");
    }
    else
    {
        printf("========== 已保存的学生记录 ==========\n");

        printf("%-15s %-12s %-8s %-8s %-8s %-8s\n",
            "学号", "姓名", "成绩1", "成绩2", "成绩3", "平均分");

        while (fscanf(fp, "%19s %29s %f %f %f %f",
            id, name, &s1, &s2, &s3, &average) == 6)       //id和name是数组，数组名代表首元素的地址，浮点数要加&
        {
            printf("%-15s %-12s %-8.1f %-8.1f %-8.1f %-8.1f\n",
                id, name, s1, s2, s3, average);
        }

        fclose(fp);
        printf("\n");
    }

    /* 第二部分：追加写入新记录 */
    fp = fopen(FILENAME, "a");

    if (fp == NULL)
    {
        printf("文件创建或打开失败，请检查文件路径！\n");
        return 1;
    }

    printf("请录入 %d 名学生的信息。\n", STUDENT_COUNT);

    for (i = 0; i < STUDENT_COUNT; i++)
    {
        printf("\n第 %d 名学生：\n", i + 1);
        printf("请输入学号 姓名 三门成绩：");

        count = scanf("%19s %29s %f %f %f",
            id, name, &s1, &s2, &s3);

        // 清理当前行剩余的输入 
        while ((ch = getchar()) != '\n' && ch != EOF);

        /* 验证输入格式 */
        if (count != 5)
        {
            printf("输入格式错误，请重新输入！\n");
            if (ch == EOF)
            {
                fclose(fp);
                return 1;
            }
            i--;           //假设第二名学生,i=1, i--变0，for循环让它加1，抵消变化，避免输入个数变化
            continue;
        }

        /* 检查成绩范围，也排除NaN */
        if (!(s1 >= 0 && s1 <= 100 &&
            s2 >= 0 && s2 <= 100 &&
            s3 >= 0 && s3 <= 100))
        {
            printf("成绩必须在0到100之间，请重新输入！\n");
            i--;
            continue;
        }

        /* 计算平均成绩 */
        average = (s1 + s2 + s3) / 3.0f;

        /* 输出计算结果 */
        printf("平均分：%.1f\n", average);

        /* 保存到文件 */
        if (fprintf(fp, "%s %s %.1f %.1f %.1f %.1f\n",
            id, name, s1, s2, s3, average) < 0)
        {
            printf("文件写入失败！\n");
            fclose(fp);
            return 1;
        }

        printf("第 %d 名学生保存成功！\n", i + 1);
    }

    /* 关闭文件并检查结果 */
    if (fclose(fp) != 0)
    {
        printf("文件关闭或保存失败！\n");
        return 1;
    }

    printf("\n========== 操作完成 ==========\n");
    printf("已追加保存 %d 名学生的信息。\n", STUDENT_COUNT);
    printf("保存文件：%s\n", FILENAME);

    return 0;
}
