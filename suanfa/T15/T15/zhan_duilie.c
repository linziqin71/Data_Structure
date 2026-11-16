#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ================= 1. 定义基础栈结构 =================
typedef struct {
    int* data;
    int top;
    int capacity;
} Stack;

// 初始化栈
Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1; // -1 表示栈空
    stack->data = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

// 压栈
void pushStack(Stack* stack, int x) {
    stack->data[++stack->top] = x;
}

// 出栈
int popStack(Stack* stack) {
    return stack->data[stack->top--];
}

// 获取栈顶元素
int peekStack(Stack* stack) {
    return stack->data[stack->top];
}

// 判断栈是否为空
bool isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

// 释放栈内存
void freeStack(Stack* stack) {
    free(stack->data);
    free(stack);
}


// ================= 2. 题目要求的 MyQueue 实现 =================

typedef struct {
    Stack* inStack;  // 输入栈
    Stack* outStack; // 输出栈
} MyQueue;


MyQueue* myQueueCreate() {
    MyQueue* obj = (MyQueue*)malloc(sizeof(MyQueue));
    // 假设最多操作 100 次，容量开 100 即可（实际工程中可动态扩容）
    obj->inStack = createStack(100);
    obj->outStack = createStack(100);
    return obj;
}

void myQueuePush(MyQueue* obj, int x) {
    // 直接压入输入栈
    pushStack(obj->inStack, x);
}

int myQueuePop(MyQueue* obj) {
    // 如果输出栈为空，把输入栈的元素全部倒过来
    if (isStackEmpty(obj->outStack)) {
        while (!isStackEmpty(obj->inStack)) {
            pushStack(obj->outStack, popStack(obj->inStack));
        }
    }
    // 从输出栈弹出
    return popStack(obj->outStack);
}

int myQueuePeek(MyQueue* obj) {
    // 和 Pop 逻辑一样，只是最后不弹出，而是获取栈顶
    if (isStackEmpty(obj->outStack)) {
        while (!isStackEmpty(obj->inStack)) {
            pushStack(obj->outStack, popStack(obj->inStack));
        }
    }
    return peekStack(obj->outStack);
}

bool myQueueEmpty(MyQueue* obj) {
    // 两个栈都为空，队列才为空
    return isStackEmpty(obj->inStack) && isStackEmpty(obj->outStack);
}

void myQueueFree(MyQueue* obj) {
    freeStack(obj->inStack);
    freeStack(obj->outStack);
    free(obj);
}

// ================= 3. 测试用的 main 函数 =================

int main() {
    printf("--- 测试开始 ---\n");

    // 1. 创建队列
    MyQueue* obj = myQueueCreate();
    printf("队列已创建。\n");

    // 2. push 1
    myQueuePush(obj, 1);
    printf("push 1 -> 队列: [1]\n");

    // 3. push 2
    myQueuePush(obj, 2);
    printf("push 2 -> 队列: [1, 2]\n");

    // 4. peek
    int p1 = myQueuePeek(obj);
    printf("peek -> 返回 %d (应为 1)\n", p1);

    // 5. pop
    int p2 = myQueuePop(obj);
    printf("pop -> 返回 %d (应为 1)\n", p2);
    printf("当前队列: [2]\n");

    // 6. empty
    bool e1 = myQueueEmpty(obj);
    printf("empty -> 返回 %s (应为 false)\n", e1 ? "true" : "false");

    // 7. pop again
    int p3 = myQueuePop(obj);
    printf("pop -> 返回 %d (应为 2)\n", p3);

    // 8. empty again
    bool e2 = myQueueEmpty(obj);
    printf("empty -> 返回 %s (应为 true)\n", e2 ? "true" : "false");

    // 9. 释放内存
    myQueueFree(obj);
    printf("队列已销毁。\n");

    printf("--- 测试结束 ---\n");

    return 0;
}