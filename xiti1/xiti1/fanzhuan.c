#include <stdio.h>

// ================= 1. 基础定义 =================
#define MAXSIZE 100
#define OK 1
#define ERROR 0

typedef int status;

// 定义顺序栈结构体
typedef struct {
    int data[MAXSIZE];
    int top; // 栈顶指针，-1表示空栈
} stack;

// ================= 2. 基础操作函数 =================
// 初始化
void initStack(stack* s) {
    s->top = -1;
}

// 判空
status stackempty(stack s) {
    return (s.top == -1) ? OK : ERROR;
}

// 入栈 (注意传指针)
status push(stack* s, int e) {
    if (s->top == MAXSIZE - 1) return ERROR;
    s->top++;
    s->data[s->top] = e;
    return OK;
}

// 出栈 (注意传指针)
status pop(stack* s, int* e) {
    if (s->top == -1) return ERROR;
    *e = s->data[s->top];
    s->top--;
    return OK;
}

// 辅助函数：从栈底到栈顶打印（不破坏栈结构，仅用于观察）
void printStackBottomToTop(stack s) {
    if (s.top == -1) {
        printf("空栈\n");
        return;
    }
    for (int i = 0; i <= s.top; i++) {
        printf("%d ", s.data[i]);
    }
    printf("\n");
}

// ================= 3. 核心反转算法 =================
// 参数必须是 stack *s，否则修改的只是副本
status algo1(stack* s) {
    int i, n, a[255];
    n = 0;

    // 步骤一：全部出栈，存入数组
    while (!stackempty(*s)) {
        n++;
        pop(s, &a[n]);
    }

    // 步骤二：重新入栈
    for (i = 1; i <= n; i++) {
        push(s, a[i]);
    }
    return OK;
}

// ================= 4. 主函数测试 =================
int main() {
    stack s;
    initStack(&s);

    // 1. 压入初始数据
    for (int i = 1; i <= 5; i++) {
        push(&s, i);
    }

    printf("=== 反转前 ===\n");
    printf("从【栈底】到【栈顶】顺序: ");
    printStackBottomToTop(s); // 打印物理存储顺序

    // 2. 执行反转
    printf("\n执行 algo1 反转算法...\n");
    algo1(&s);

    printf("\n=== 反转后 ===\n");
    printf("从【栈底】到【栈顶】顺序: ");
    printStackBottomToTop(s); // 再次打印物理存储顺序，你会看到完全反转了！

    return 0;
}