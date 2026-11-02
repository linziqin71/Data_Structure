
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{
    char password[128];
    int reused, ch, triggered = 0, same = 1;
    size_t len, i;

    const char* weak[] = {
        "password", "123456", "qwerty",
        "12345678", "admin"
    };

    printf("=== 密码策略检查器（教学用途）===\n");
    printf("请使用模拟密码，不要输入真实账户密码。\n");

    /* 输入模拟密码 */
    printf("请输入模拟密码（不超过127字节）：");

    if (fgets(password, sizeof(password), stdin) == NULL)
        return 1;

    len = strlen(password);

    /* 删除末尾的换行符 */
    if (len && password[len - 1] == '\n')
        password[--len] = '\0';
    else if (!feof(stdin))
    {
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        printf("输入过长，已拒绝。\n");
        return 1;
    }

    /* 判断密码是否为空 */
    if (len == 0)
    {
        printf("模拟密码不能为空。\n");
        return 1;
    }

    /* 输入是否重复使用 */
    printf("是否在多个网站使用同一密码？(1是/0否)：");

    if (scanf("%d", &reused) != 1 ||
        (reused != 0 && reused != 1))
    {
        printf("重复使用标志无效。\n");
        return 1;
    }

    /* 规则1：密码长度检查 */
    if (len < 15)
    {
        printf("触发规则1：长度不足15字节"
            "（本程序输入按单字节字符演示）。\n");
        triggered++;
    }

    /* 规则2：检查常见弱密码 */
    for (i = 0; i < sizeof(weak) / sizeof(weak[0]); i++)
    {
        if (strcmp(password, weak[i]) == 0)
        {
            printf("触发规则2：匹配教学用常见弱密码样例。\n");
            triggered++;
            break;
        }
    }

    /* 规则3：密码重复使用 */
    if (reused)
    {
        printf("触发规则3：在不同网站重复使用密码。\n");
        triggered++;
    }

    /* 规则4：检查重复字符 */
    for (i = 1; i < len; i++)
    {
        if (password[i] != password[0])
        {
            same = 0;
            break;
        }
    }

    if (same)
    {
        printf("触发规则4：密码由单一字符重复构成。\n");
        triggered++;
    }

    /* 输出综合结果 */
    if (!triggered)
        printf("未触发本实验设置的4项检查规则。\n");
    else
        printf("共触发%d项规则，需要改进。\n", triggered);

    printf("这仅是基础规则演示，"
        "未触发不等于密码绝对安全。\n");

    return 0;
}
