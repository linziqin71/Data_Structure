
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100

// 哈夫曼树结点
typedef struct {
    char ch;
    int weight;
    int parent;
    int left;
    int right;
} HTNode;

HTNode HT[2 * MAXN];
char HC[MAXN][MAXN];
int n = 0;

// 选取两个权值最小且没有父结点的结点
void Select(int end, int* s1, int* s2) {
    *s1 = *s2 = 0;

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

// 生成每个字符的哈夫曼编码
void CreateCode() {
    for (int i = 1; i <= n; i++) {
        char temp[MAXN];
        int len = 0;
        int c = i;
        int p = HT[c].parent;

        while (p != 0) {
            if (HT[p].left == c)
                temp[len++] = '0';
            else
                temp[len++] = '1';

            c = p;
            p = HT[c].parent;
        }

        // 将从叶子到根得到的编码逆序
        for (int j = 0; j < len; j++)
            HC[i][j] = temp[len - 1 - j];

        HC[i][len] = '\0';
    }
}

// 保存哈夫曼树到文件
void SaveTree() {
    FILE* fp = fopen("hfmTree", "w");

    if (fp == NULL) {
        printf("保存文件失败！\n");
        return;
    }

    fprintf(fp, "%d\n", n);

    for (int i = 1; i <= 2 * n - 1; i++) {
        fprintf(fp, "%d %d %d %d %d\n",
            (int)(unsigned char)HT[i].ch,
            HT[i].weight,
            HT[i].parent,
            HT[i].left,
            HT[i].right);
    }

    fclose(fp);
    printf("哈夫曼树已保存到 hfmTree\n");
}

// 从文件读取哈夫曼树
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

    n = t;

    for (int i = 1; i <= 2 * n - 1; i++) {
        if (fscanf(fp, "%d%d%d%d%d",
            &ch,
            &HT[i].weight,
            &HT[i].parent,
            &HT[i].left,
            &HT[i].right) != 5) {
            fclose(fp);
            n = 0;
            return 0;
        }
        HT[i].ch = (char)ch;
    }

    fclose(fp);
    CreateCode();
    return 1;
}

// 初始化并建立哈夫曼树
void Init() {
    int s1, s2;

    printf("请输入字符个数n：");
    if (scanf("%d", &n) != 1 ||
        n < 2 || n > MAXN) {
        printf("字符个数不合法！\n");
        n = 0;
        return;
    }

    memset(HT, 0, sizeof(HT));

    for (int i = 1; i <= n; i++) {
        printf("请输入第%d个字符及权值：", i);

        if (scanf(" %c%d",
            &HT[i].ch,
            &HT[i].weight) != 2 ||
            HT[i].weight <= 0) {
            printf("输入错误！\n");
            n = 0;
            return;
        }

        for (int j = 1; j < i; j++) {
            if (HT[j].ch == HT[i].ch) {
                printf("字符不能重复！\n");
                n = 0;
                return;
            }
        }
    }

    // 逐步合并两个最小权值结点
    for (int i = n + 1; i <= 2 * n - 1; i++) {
        Select(i - 1, &s1, &s2);

        HT[s1].parent = i;
        HT[s2].parent = i;

        HT[i].left = s1;
        HT[i].right = s2;

        HT[i].weight =
            HT[s1].weight + HT[s2].weight;
    }

    CreateCode();
    SaveTree();

    printf("\n哈夫曼编码表：\n");

    for (int i = 1; i <= n; i++)
        printf("%c：%s\n", HT[i].ch, HC[i]);
}

// 检查哈夫曼树是否已建立
int Ready() {
    if (n >= 2)
        return 1;

    if (LoadTree())
        return 1;

    printf("请先初始化哈夫曼树！\n");
    return 0;
}

// 编码：读取正文文件，输出CodeFile
void Encode() {
    FILE* in;
    FILE* out;
    char filename[100];
    int ch, i;
    int ok = 1;

    if (!Ready())
        return;

    printf("请输入正文文件名：");
    scanf("%99s", filename);

    in = fopen(filename, "r");

    if (in == NULL) {
        printf("正文文件打开失败！\n");
        return;
    }

    // 先检查字符是否都属于字符集
    while ((ch = fgetc(in)) != EOF) {
        int found = 0;

        for (i = 1; i <= n; i++) {
            if ((unsigned char)HT[i].ch == ch) {
                found = 1;
                break;
            }
        }

        if (!found) {
            ok = 0;
            break;
        }
    }

    if (!ok) {
        printf("正文中存在未定义字符！\n");
        fclose(in);
        return;
    }

    rewind(in);

    out = fopen("CodeFile", "w");

    if (out == NULL) {
        printf("CodeFile创建失败！\n");
        fclose(in);
        return;
    }

    printf("编码结果：");

    while ((ch = fgetc(in)) != EOF) {
        for (i = 1; i <= n; i++) {
            if ((unsigned char)HT[i].ch == ch) {
                printf("%s", HC[i]);
                fputs(HC[i], out);
                break;
            }
        }
    }

    printf("\n编码已保存到CodeFile\n");

    fclose(in);
    fclose(out);
}

// 译码：读取CodeFile，输出TextFile
void Decode() {
    FILE* in;
    FILE* out;
    int bit;
    int root, p;
    int valid = 1;

    if (!Ready())
        return;

    in = fopen("CodeFile", "r");

    if (in == NULL) {
        printf("CodeFile不存在！\n");
        return;
    }

    // 第一遍检查编码是否合法
    root = 2 * n - 1;
    p = root;

    while ((bit = fgetc(in)) != EOF) {
        if (bit != '0' && bit != '1') {
            valid = 0;
            break;
        }

        if (bit == '0')
            p = HT[p].left;
        else
            p = HT[p].right;

        if (p <= 0 || p >= 2 * n) {
            valid = 0;
            break;
        }

        if (HT[p].left == 0 &&
            HT[p].right == 0)
            p = root;
    }

    if (p != root)
        valid = 0;

    if (!valid) {
        printf("编码文件不合法或不完整！\n");
        fclose(in);
        return;
    }

    rewind(in);

    out = fopen("TextFile", "w");

    if (out == NULL) {
        printf("TextFile创建失败！\n");
        fclose(in);
        return;
    }

    p = root;
    printf("译码结果：");

    while ((bit = fgetc(in)) != EOF) {
        if (bit == '0')
            p = HT[p].left;
        else
            p = HT[p].right;

        // 到达叶子结点，输出对应字符
        if (HT[p].left == 0 &&
            HT[p].right == 0) {
            printf("%c", HT[p].ch);
            fputc(HT[p].ch, out);
            p = root;
        }
    }

    printf("\n译码已保存到TextFile\n");

    fclose(in);
    fclose(out);
}

int main() {
    int choice;

    while (1) {
        printf("\n===== 哈夫曼编译码系统 =====\n");
        printf("1. 初始化哈夫曼树\n");
        printf("2. 编码\n");
        printf("3. 译码\n");
        printf("0. 退出\n");
        printf("请选择：");

        if (scanf("%d", &choice) != 1)
            break;

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
            return 0;
        default:
            printf("输入错误，请重新选择！\n");
        }
    }

    return 0;
}
