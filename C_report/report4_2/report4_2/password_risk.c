
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
    int choice, length, mixed, digit, mfa, reused, risk, ch;

    printf("=== 账号安全风险评分器（教学模型）===\n");
    printf("本程序不要求输入真实密码，只输入五项安全特征。\n");

    while (1)
    {
        printf("\n1. 评估账号风险   0. 退出\n");
        printf("请选择：");

        if (scanf("%d", &choice) != 1)
        {
            if (feof(stdin)) break;

            printf("菜单输入无效。\n");

            while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        while ((ch = getchar()) != '\n' && ch != EOF) {}

        if (choice == 0)
            break;

        if (choice != 1)
        {
            printf("无效选项。\n");
            continue;
        }

        /* 输入五项安全信息 */
        printf("依次输入：密码长度 大小写兼有(0/1) "
            "含数字(0/1) 开启双因素认证(0/1) "
            "跨网站重复使用(0/1)\n");

        if (scanf("%d %d %d %d %d",
            &length, &mixed, &digit, &mfa, &reused) != 5)
        {
            printf("输入格式错误，本次不评分。\n");

            while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        while ((ch = getchar()) != '\n' && ch != EOF) {}

        /* 检查输入范围 */
        if (length < 0 || length > 100000 ||
            (mixed != 0 && mixed != 1) ||
            (digit != 0 && digit != 1) ||
            (mfa != 0 && mfa != 1) ||
            (reused != 0 && reused != 1))
        {
            printf("输入范围错误：长度应为0~100000，"
                "其余项仅可输入0或1。\n");
            continue;
        }

        /* 初始化风险分数 */
        risk = 0;

        /* 密码长度评分 */
        if (length < 8)
            risk += 30;
        else if (length < 15)
            risk += 20;

        /* 其他风险因素评分 */
        if (!mixed) risk += 10;
        if (!digit) risk += 10;
        if (!mfa) risk += 20;
        if (reused) risk += 30;

        /* 风险分数限制在100以内 */
        if (risk > 100)
            risk = 100;

        /* 输出风险等级 */
        printf("风险分数：%d/100；等级：%s\n", risk,
            risk >= 60 ? "高风险" :
            risk >= 30 ? "中风险" : "低风险");

        /* 输出相应建议 */
        if (length < 15)
            printf("建议：使用更长的密码或口令短语。\n");

        if (!mfa)
            printf("建议：开启双因素认证。\n");

        if (reused)
            printf("建议：不同网站使用不同密码。\n");

        printf("注：该分数为课程自拟模型，不代表真实攻击概率。\n");
    }

    return 0;
}
