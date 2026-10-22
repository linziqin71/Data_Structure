#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
    FILE* src, * dst;
    char buffer[256];
    int lines = 0, hasData = 0;
    int lastChar = '\n';
    size_t i;

    /* 打开原文件 */
    src = fopen("study_log.txt", "r");

    if (src == NULL)
    {
        printf("原文件打开失败！\n");
        return 1;
    }

    /* 创建备份文件 */
    dst = fopen("backup.txt", "w");

    if (dst == NULL)
    {
        printf("备份文件创建失败！\n");
        fclose(src);
        return 1;
    }

    /* 按段读取并复制文件内容 */
    while (fgets(buffer, sizeof(buffer), src) != NULL)
    {
        if (fputs(buffer, dst) == EOF)
        {
            printf("文件写入失败！\n");
            fclose(src);
            fclose(dst);
            return 1;
        }

        /* 统计读取内容中的换行符 */
        for (i = 0; buffer[i] != '\0'; i++)
        {
            hasData = 1;
            lastChar = buffer[i];

            if (buffer[i] == '\n')
                lines++;
        }
    }

    /* 最后一行没有换行符时补计一行 */
    if (hasData && lastChar != '\n')
        lines++;

    /* 检查读取和关闭是否成功 */
    if (ferror(src))
    {
        printf("原文件读取失败！\n");
        fclose(src);
        fclose(dst);
        return 1;
    }

    if (fclose(src) != 0)
    {
        fclose(dst);
        return 1;
    }

    if (fclose(dst) != 0)
    {
        printf("备份文件保存失败！\n");
        return 1;
    }

    printf("文件备份成功！\n");
    printf("文件总行数：%d\n", lines);

    return 0;
}
