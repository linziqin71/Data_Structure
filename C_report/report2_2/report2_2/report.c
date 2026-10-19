
#include <stdio.h>
#include <limits.h>

int main(void)
{
    long long dailyGB, totalGB, totalBytes;
    int days;
    float timeF;
    double speed, timeD;

    /* 输入每日数据量、带宽和保留天数 */
    printf("每日数据量(GB)：");
    if (scanf_s("%lld", &dailyGB) != 1) return 1;

    printf("上传带宽(Mbps)：");
    if (scanf_s("%lf", &speed) != 1) return 1;

    printf("保留天数：");
    if (scanf_s("%d", &days) != 1) return 1;

    /* 检查输入及计算范围 */
    if (dailyGB <= 0 || days <= 0 ||
        !(speed > 0 && speed < 1e100))
    {
        printf("输入无效！\n");
        return 1;
    }

    /* 避免总字节数超出long long范围 */
    if (dailyGB > LLONG_MAX / 1000000000LL / days)
    {
        printf("数据过大，无法安全计算！\n");
        return 1;
    }

    /* 分别使用float和double计算上传时间 */
    timeF = (float)dailyGB * 8000.0f / (float)speed;
    timeD = (double)dailyGB * 8000.0 / speed;

    /* 计算存储容量 */
    totalGB = dailyGB * days;
    totalBytes = totalGB * 1000000000LL;

    /* 输出计算结果 */
    printf("\nfloat时间：%.2f 秒\n", timeF);
    printf("double时间：%.2f 秒\n", timeD);
    printf("高精度float：%.9f\n", timeF);
    printf("高精度double：%.9f\n", timeD);

    printf("总容量：%lld GB\n", totalGB);
    printf("总字节数：%lld Byte\n", totalBytes);

    /* 验证整数类型的存储能力 */
    printf("\nint大小：%zu 字节\n", sizeof(int));
    printf("long long大小：%zu 字节\n",
        sizeof(long long));
    printf("int最大值：%d\n", INT_MAX);

    if (totalBytes > INT_MAX)
        printf("验证：int无法存储该字节数。\n");
    else
        printf("验证：int可以存储该字节数。\n");

    return 0;
}
