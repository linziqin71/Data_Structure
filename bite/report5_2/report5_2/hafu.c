
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100
#define MAXCODE (2 * MAXN)

// 哈夫曼树结点
typedef struct {
    char ch;
    int weight;
    int parent;
    int left;
    int right;
} HTNode;

// 全局变量
HTNode HT[2 * MAXN];
char HC[MAXN + 1][MAXCODE];
int n = 0;
int treeReady = 0;

// 选取两个权值最小且没有父结点的结点
void Select(int end, int* s1, int* s2) {
    *s1 = 0;
    *s2 = 0;

    for (int i = 1; i <= end; i++) {
        if (HT[i].parent != 0)
            continue;

        if (*s1 == 0 ||
            HT[i].weight < HT[*s1].weight) {
            *s2 = *s1;
            *s1 = i;
        }
        else if (*s2 == 0 ||
            HT[i].weight < HT[*s2].weight) {
            *s2 = i;
        }
    }
}

// 生成哈夫曼编码
void CreateCode() {
    for (int i = 1; i <= n; i++) {
        char temp[MAXCODE];
        int len = 0;
        int c = i;
        int p = HT[c].parent;

        // 从叶子结点向根结点查找
        while (p != 0) {
            if (HT[p].left == c)
                temp[len++] = '0';
            else
                temp[len++] = '1';

            c = p;
            p = HT[c].parent;
        }

        // 将编码逆序保存
        for (int j = 0; j < len; j++)
            HC[i][j] = temp[len - 1 - j];

        HC[i][len] = '\0';
    }
}

// 保存哈夫曼树
int SaveTree() {
    FILE* fp = fopen("hfmTree", "w");

    if (fp == NULL) {
        printf("错误：无法创建hfmTree！\n");
        return 0;
    }

    if (fprintf(fp, "%d\n", n) < 0) {
        fclose(fp);
        return 0;
    }

    for (int i = 1; i <= 2 * n - 1; i++) {
        if (fprintf(fp, "%d %d %d %d %d\n",
            (int)(unsigned char)HT[i].ch,
            HT[i].weight,
            HT[i].parent,
            HT[i].left,
            HT[i].right) < 0) {
            fclose(fp);
            return 0;
        }
    }

    if (fclose(fp) != 0) {
        printf("错误：哈夫曼树保存失败！\n");
        return 0;
    }

    printf("哈夫曼树已保存到hfmTree\n");
    return 1;
}

// 检查哈夫曼树结构是否合法
int CheckTree() {
    int root = 2 * n - 1;
    int count[2 * MAXN] = { 0 };

    if (n < 2 || n > MAXN)
        return 0;

    // 检查叶子结点
    for (int i = 1; i <= n; i++) {
        if (HT[i].weight <= 0 ||
            HT[i].left != 0 ||
            HT[i].right != 0)
            return 0;

        for (int j = 1; j < i; j++) {
            if (HT[i].ch == HT[j].ch)
                return 0;
        }
    }

    // 检查非叶子结点
    for (int i = n + 1; i <= root; i++) {
        int l = HT[i].left;
        int r = HT[i].right;

        if (l < 1 || r < 1 ||
            l >= i || r >= i || l == r)
            return 0;

        if (HT[l].parent != i ||
            HT[r].parent != i)
            return 0;

        if (HT[i].weight !=
            HT[l].weight + HT[r].weight)
            return 0;

        count[l]++;
        count[r]++;
    }

    // 检查根结点和父子关系
    if (HT[root].parent != 0)
        return 0;

    for (int i = 1; i < root; i++) {
        if (count[i] != 1 ||
            HT[i].parent <= i ||
            HT[i].parent > root)
            return 0;
    }

    return 1;
}

// 从文件加载哈夫曼树
int LoadTree() {
    FILE* fp = fopen("hfmTree", "r");
    int t, ch;

    if (fp == NULL)
        return 0;

    if (fscanf(fp, "%d", &t) != 1 ||
        t < 2 || t > MAXN) {
        fclose(fp);
        return 0;
    }

    memset(HT, 0, sizeof(HT));
    n = t;

    for (int i = 1; i <= 2 * n - 1; i++) {
        if (fscanf(fp, "%d%d%d%d%d",
            &ch,
            &HT[i].weight,
            &HT[i].parent,
            &HT[i].left,
            &HT[i].right) != 5 ||
            ch < 0 || ch > 255) {
            fclose(fp);
            n = 0;
            return 0;
        }

        HT[i].ch = (char)ch;
    }

    fclose(fp);

    if (!CheckTree()) {
        n = 0;
        return 0;
    }

    CreateCode();
    treeReady = 1;

    printf("已从hfmTree加载哈夫曼树！\n");
    return 1;
}

// 初始化哈夫曼树
void Init() {
    int s1, s2;
    int newN;

    printf("请输入字符个数n：");

    if (scanf("%d", &newN) != 1 ||
        newN < 2 || newN > MAXN) {
        printf("错误：字符个数应为2~100！\n");
        return;
    }

    // 只有开始有效初始化后才清除旧树
    n = newN;
    treeReady = 0;
    memset(HT, 0, sizeof(HT));
    memset(HC, 0, sizeof(HC));

    for (int i = 1; i <= n; i++) {
        printf("请输入第%d个字符及权值：", i);

        if (scanf(" %c%d",
            &HT[i].ch,
            &HT[i].weight) != 2 ||
            HT[i].weight <= 0) {
            printf("错误：字符或权值不合法！\n");
            n = 0;
            return;
        }

        // 检查字符是否重复
        for (int j = 1; j < i; j++) {
            if (HT[i].ch == HT[j].ch) {
                printf("错误：字符不能重复！\n");
                n = 0;
                return;
            }
        }
    }

    // 构造哈夫曼树
    for (int i = n + 1; i <= 2 * n - 1; i++) {
        Select(i - 1, &s1, &s2);

        HT[s1].parent = i;
        HT[s2].parent = i;

        HT[i].left = s1;
        HT[i].right = s2;

        HT[i].weight =
            HT[s1].weight + HT[s2].weight;
    }

    if (!CheckTree()) {
        printf("错误：哈夫曼树构造失败！\n");
        n = 0;
        return;
    }

    CreateCode();
    treeReady = 1;

    printf("\n哈夫曼编码表：\n");
    printf("字符\t权值\t编码\n");

    for (int i = 1; i <= n; i++) {
        printf("%c\t%d\t%s\n",
            HT[i].ch,
            HT[i].weight,
            HC[i]);
    }

    if (!SaveTree())
        printf("警告：树已建立，但文件保存失败！\n");
}

// 检查哈夫曼树是否可用
int Ready() {
    if (treeReady)
        return 1;

    if (LoadTree())
        return 1;

    printf("请先初始化哈夫曼树！\n");
    return 0;
}

// 查找字符对应的结点编号
int FindChar(int ch) {
    for (int i = 1; i <= n; i++) {
        if ((unsigned char)HT[i].ch == ch)
            return i;
    }

    return 0;
}

// 编码
void Encode() {
    FILE* in;
    FILE* out;
    char filename[100];
    int ch, index;
    long count = 0;
    long bits = 0;
    int error = 0;

    if (!Ready())
        return;

    printf("请输入正文文件名：");

    if (scanf("%99s", filename) != 1)
        return;

    in = fopen(filename, "rb");

    if (in == NULL) {
        printf("错误：正文文件打开失败！\n");
        return;
    }

    // 第一遍：检查正文内容
    while ((ch = fgetc(in)) != EOF) {
        count++;

        if (FindChar(ch) == 0) {
            printf("错误：第%ld个字符不在字符集中！\n",
                count);
            fclose(in);
            return;
        }
    }

    if (ferror(in)) {
        printf("错误：正文文件读取失败！\n");
        fclose(in);
        return;
    }

    if (count == 0) {
        printf("错误：正文文件为空，无法编码！\n");
        fclose(in);
        return;
    }

    rewind(in);

    out = fopen("CodeFile", "w");

    if (out == NULL) {
        printf("错误：无法创建CodeFile！\n");
        fclose(in);
        return;
    }

    printf("\n编码结果：");

    // 第二遍：正式编码
    while ((ch = fgetc(in)) != EOF) {
        index = FindChar(ch);

        if (fputs(HC[index], out) == EOF) {
            error = 1;
            break;
        }

        printf("%s", HC[index]);
        bits += (long)strlen(HC[index]);
    }

    if (ferror(in))
        error = 1;

    fclose(in);

    if (fclose(out) != 0)
        error = 1;

    if (error) {
        printf("\n错误：编码文件写入失败！\n");
        return;
    }

    printf("\n\n编码成功！\n");
    printf("原始字符数：%ld\n", count);
    printf("编码位数：%ld\n", bits);
    printf("结果已保存到CodeFile\n");
}

// 译码
void Decode() {
    FILE* in;
    FILE* out;
    int bit;
    int root, p;
    int valid = 1;
    int count = 0;
    int error = 0;

    if (!Ready())
        return;

    in = fopen("CodeFile", "r");

    if (in == NULL) {
        printf("错误：CodeFile不存在！\n");
        return;
    }

    root = 2 * n - 1;
    p = root;

    // 第一遍：检查编码是否合法
    while ((bit = fgetc(in)) != EOF) {
        count++;

        if (bit != '0' && bit != '1') {
            valid = 0;
            break;
        }

        if (bit == '0')
            p = HT[p].left;
        else
            p = HT[p].right;

        if (p <= 0 || p > root) {
            valid = 0;
            break;
        }

        // 遇到叶子结点，返回根结点
        if (HT[p].left == 0 &&
            HT[p].right == 0)
            p = root;
    }

    if (ferror(in))
        valid = 0;

    if (count == 0) {
        printf("错误：CodeFile为空！\n");
        fclose(in);
        return;
    }

    if (!valid || p != root) {
        printf("错误：编码不合法或不完整！\n");
        fclose(in);
        return;
    }

    rewind(in);

    out = fopen("TextFile", "wb");

    if (out == NULL) {
        printf("错误：无法创建TextFile！\n");
        fclose(in);
        return;
    }

    p = root;

    printf("\n译码结果：");

    // 第二遍：正式译码
    while ((bit = fgetc(in)) != EOF) {
        if (bit == '0')
            p = HT[p].left;
        else
            p = HT[p].right;

        if (HT[p].left == 0 &&
            HT[p].right == 0) {
            if (fputc((unsigned char)HT[p].ch,
                out) == EOF) {
                error = 1;
                break;
            }

            printf("%c", HT[p].ch);
            p = root;
        }
    }

    if (ferror(in))
        error = 1;

    fclose(in);

    if (fclose(out) != 0)
        error = 1;

    if (error) {
        printf("\n错误：译码文件写入失败！\n");
        return;
    }

    printf("\n\n译码成功！\n");
    printf("结果已保存到TextFile\n");
}

// 主函数
int main() {
    int choice;

    while (1) {
        printf("\n===== 哈夫曼编译码系统 =====\n");
        printf("1. 初始化哈夫曼树\n");
        printf("2. 编码\n");
        printf("3. 译码\n");
        printf("0. 退出\n");
        printf("请选择：");

        if (scanf("%d", &choice) != 1) {
            printf("输入结束或格式错误！\n");
            break;
        }

        switch (choice) {
        case 1:
            Init();
            break;

        case 2:
            Encode();
            break;

        case 3:
            Decode();
            break;

        case 0:
            printf("程序结束！\n");
            return 0;

        default:
            printf("输入错误，请重新选择！\n");
        }
    }

    return 0;
}
