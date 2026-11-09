
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX_SIZE 100

/* 函数声明 */
unsigned int checksum(unsigned char data[], int n);
void inputData(unsigned char data[], int* n);
void showData(unsigned char data[], int n);
void testChecksum(void);

int main(void)
{
    unsigned char data[MAX_SIZE];
    int n = 0;
    int choice;

    while (1)
    {
        printf("\n===== 数据校验工具 =====\n");
        printf("1. 手动输入并计算校验值\n");
        printf("2. 运行三组对照实验\n");
        printf("0. 退出程序\n");
        printf("请选择：");

        if (scanf("%d", &choice) != 1)
        {
            printf("菜单输入错误！\n");
            return 1;
        }

        if (choice == 0)
            break;

        switch (choice)
        {
        case 1:
            inputData(data, &n);

            if (n > 0)
            {
                showData(data, n);

                printf("校验值(十进制)：%u\n",
                    checksum(data, n));

                printf("校验值(十六进制)：%02X\n",
                    checksum(data, n));
            }
            break;

        case 2:
            testChecksum();
            break;

        default:
            printf("菜单选项无效！\n");
        }
    }

    return 0;
}

/* 函数1：计算8位累加和校验值 */
unsigned int checksum(unsigned char data[], int n)
{
    unsigned int sum = 0;
    int i;

    for (i = 0; i < n; i++)
    {
        sum += data[i];
    }

    return sum % 256;
}

/* 函数2：输入字节数据 */
void inputData(unsigned char data[], int* n)
{
    int i, value;

    printf("请输入字节个数(1~100)：");

    if (scanf("%d", n) != 1 ||
        *n < 1 || *n > MAX_SIZE)
    {
        printf("字节个数无效！\n");
        *n = 0;
        return;
    }

    for (i = 0; i < *n; i++)
    {
        printf("请输入第%d个字节(0~255)：", i + 1);

        if (scanf("%d", &value) != 1 ||
            value < 0 || value > 255)
        {
            printf("输入错误，字节必须在0~255之间！\n");
            *n = 0;
            return;
        }

        data[i] = (unsigned char)value;
    }
}

/* 函数3：显示字节数据 */
void showData(unsigned char data[], int n)
{
    int i;

    printf("数据内容：");

    for (i = 0; i < n; i++)
    {
        printf("%u ", (unsigned int)data[i]);
    }

    printf("\n");
}

/* 函数4：执行三组校验对照实验 */
void testChecksum(void)
{
    unsigned char original[3][4] = {
        {10, 20, 30, 40},
        {100, 50, 25, 10},
        {255, 0, 128, 64}
    };

    unsigned char modified[4];
    unsigned int before, after;
    int i, j;

    printf("\n===== 三组单字节修改实验 =====\n");

    for (i = 0; i < 3; i++)
    {
        /* 复制原始数据 */
        for (j = 0; j < 4; j++)
        {
            modified[j] = original[i][j];
        }

        /* 修改其中一个字节 */
        modified[i] = (unsigned char)(
            (modified[i] + 1) % 256
            );

        /* 分别计算修改前后的校验值 */
        before = checksum(original[i], 4);
        after = checksum(modified, 4);

        printf("\n第%d组实验：\n", i + 1);

        printf("原始数据：");
        showData(original[i], 4);

        printf("修改数据：");
        showData(modified, 4);

        printf("原校验值：%u\n", before);
        printf("新校验值：%u\n", after);

        if (before != after)
            printf("检测到数据变化！\n");
        else
            printf("校验值相同，未检测到变化！\n");
    }
}
