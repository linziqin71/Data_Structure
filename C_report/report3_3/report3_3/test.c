#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define FILE_NAME "study.csv"
#define COUNT 2
#define SIZE 256

typedef struct {
    char date[11];
    int minutes;
    int problems;
    char note[SIZE];
} Record;

/* 读取一行：1成功，0到达文件末尾，-1表示内容过长 */
int read_line(FILE* fp, char* s, int size)
{
    int c;
    size_t len;
    if (fgets(s, size, fp) == NULL) return 0;
    len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
        if (len > 1 && s[len - 2] == '\r') s[len - 2] = '\0';
        return 1;
    }
    if (feof(fp)) return 1;
    /* 如果剩余恰好是行结束符，可以接受；否则拒绝过长行 */
    c = fgetc(fp);
    if (c == '\n' || c == EOF) return 1;
    while (c != '\n' && c != EOF) c = fgetc(fp);
    return -1;
}

/* 将非负整数字符串转换为int，拒绝负数、字母和越界 */
int parse_int(const char* s, int* result)
{
    long value;
    char* end;
    const char* p;
    if (*s == '\0') return 0;
    for (p = s; *p; p++)
        if (*p < '0' || *p > '9') return 0;
    value = strtol(s, &end, 10);
    if (*end != '\0' || value > INT_MAX) return 0;
    *result = (int)value;
    return 1;
}

/* 本课程的简化规则：不支持引号、逗号和换行出现在字段中 */
int valid_note(const char* s)
{
    return s[0] != '\0' && strchr(s, ',') == NULL &&
        strchr(s, '"') == NULL && strchr(s, '\r') == NULL;
}

int valid_date(const char* s)
{
    int i;
    if (strlen(s) != 10 || s[4] != '-' || s[7] != '-') return 0;
    for (i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (s[i] < '0' || s[i] > '9') return 0;
    }
    return 1; /* 只检查格式，不判断实际日期是否存在 */
}

/* 严格要求一行只有四个字段，不能多一个逗号 */
int parse_csv(char* line, Record* r)
{
    char* p1 = strchr(line, ',');
    char* p2, * p3;
    if (!p1) return 0;
    *p1++ = '\0';
    p2 = strchr(p1, ',');
    if (!p2) return 0;
    *p2++ = '\0';
    p3 = strchr(p2, ',');
    if (!p3) return 0;
    *p3++ = '\0';
    if (strchr(p3, ',') || strlen(line) >= sizeof(r->date) ||
        strlen(p3) >= sizeof(r->note)) return 0;
    if (!valid_date(line) || !parse_int(p1, &r->minutes) ||
        !parse_int(p2, &r->problems) || !valid_note(p3)) return 0;
    strcpy(r->date, line);
    strcpy(r->note, p3);
    return 1;
}

int main(void)
{
    Record original[COUNT], loaded[COUNT];
    FILE* fp;
    char line[SIZE], number[SIZE];
    int i, status, ok = 1;

    puts("===== 简化 CSV 交换器 =====");
    puts("规则：4列 date,minutes,problems,note；备注允许空格，不允许逗号和双引号。");
    for (i = 0; i < COUNT; i++) {
        printf("\n第%d条记录：\n", i + 1);
        printf("日期(YYYY-MM-DD)：");
        status = read_line(stdin, line, SIZE);
        if (status != 1 || !valid_date(line)) { puts("日期格式错误！"); return 1; }
        strcpy(original[i].date, line);

        printf("学习分钟数：");
        status = read_line(stdin, number, SIZE);
        if (status != 1 || !parse_int(number, &original[i].minutes)) {
            puts("分钟数必须是非负整数！"); return 1;
        }
        printf("完成题数：");
        status = read_line(stdin, number, SIZE);
        if (status != 1 || !parse_int(number, &original[i].problems)) {
            puts("题数必须是非负整数！"); return 1;
        }
        printf("备注（可包含空格）：");
        status = read_line(stdin, line, SIZE);
        if (status != 1 || !valid_note(line)) {
            puts("备注不允许为空，也不允许出现逗号或双引号，拒绝保存！");
            return 1;
        }
        strcpy(original[i].note, line);
    }

    /* 输入全部通过检查后再写文件 */
    fp = fopen(FILE_NAME, "w");
    if (fp == NULL) { perror("无法创建CSV文件"); return 1; }
    if (fprintf(fp, "date,minutes,problems,note\n") < 0) ok = 0;
    for (i = 0; i < COUNT && ok; i++) {
        if (fprintf(fp, "%s,%d,%d,%s\n", original[i].date,
            original[i].minutes, original[i].problems, original[i].note) < 0)
            ok = 0;
    }
    if (fclose(fp) != 0) ok = 0;
    if (!ok) { puts("写入CSV文件失败！"); return 1; }
    puts("\n写入成功，开始重新读取并逐条核对……");

    fp = fopen(FILE_NAME, "r");
    if (fp == NULL) { perror("无法重新打开CSV文件"); return 1; }
    status = read_line(fp, line, SIZE);
    if (status != 1 || strcmp(line, "date,minutes,problems,note") != 0) {
        puts("表头核验失败！"); fclose(fp); return 1;
    }
    for (i = 0; i < COUNT; i++) {
        status = read_line(fp, line, SIZE);
        if (status != 1 || !parse_csv(line, &loaded[i])) {
            printf("第%d条记录读取失败或格式异常！\n", i + 1);
            fclose(fp); return 1;
        }
        ok = strcmp(original[i].date, loaded[i].date) == 0 &&
            original[i].minutes == loaded[i].minutes &&
            original[i].problems == loaded[i].problems &&
            strcmp(original[i].note, loaded[i].note) == 0;
        printf("第%d条：%s,%d,%d,%s -> %s\n", i + 1,
            loaded[i].date, loaded[i].minutes, loaded[i].problems,
            loaded[i].note, ok ? "一致" : "不一致");
        if (!ok) { fclose(fp); return 1; }
    }
    status = read_line(fp, line, SIZE);
    if (status != 0 || ferror(fp)) {
        puts("发现多余记录或读取异常！"); fclose(fp); return 1;
    }
    if (fclose(fp) != 0) { puts("文件关闭失败！"); return 1; }
    puts("全部字段核对一致，实验成功！");
    return 0;
}
