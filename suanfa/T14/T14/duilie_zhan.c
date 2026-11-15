#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* ==========================================
 * 1. 底层队列的数据结构及操作实现
 * ========================================== */
#define MAX_SIZE 1000 // 假设最多操作 1000 个元素，足够应对测试用例

 // 定义一个简单的队列结构
typedef struct {
    int data[MAX_SIZE];
    int front;
    int rear;
    int size;
} Queue;

// 初始化队列
Queue* queueCreate() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    return q;
}

// 入队 (push to back)
void queuePush(Queue* obj, int x) {
    obj->data[obj->rear] = x;
    obj->rear = (obj->rear + 1) % MAX_SIZE;
    obj->size++;
}

// 出队 (pop from front)
int queuePop(Queue* obj) {
    int val = obj->data[obj->front];
    obj->front = (obj->front + 1) % MAX_SIZE;
    obj->size--;
    return val;
}

// 获取队头元素 (peek from front)
int queuePeek(Queue* obj) {
    return obj->data[obj->front];
}

// 获取队列大小 (size)
int queueSize(Queue* obj) {
    return obj->size;
}

// 判断队列是否为空 (is empty)
bool queueEmpty(Queue* obj) {
    return obj->size == 0;
}

// 释放队列
void queueFree(Queue* obj) {
    free(obj);
}

/* ==========================================
 * 2. 用两个队列实现栈的数据结构及操作
 * ========================================== */
typedef struct {
    Queue* q1;
    Queue* q2;
} MyStack;

// 创建栈
MyStack* myStackCreate() {
    MyStack* obj = (MyStack*)malloc(sizeof(MyStack));
    obj->q1 = queueCreate();
    obj->q2 = queueCreate();
    return obj;
}

// 压入栈顶
void myStackPush(MyStack* obj, int x) {
    // 哪个队列不为空，就压入哪个队列。如果都为空，压入 q1
    if (!queueEmpty(obj->q1)) {
        queuePush(obj->q1, x);
    }
    else {
        queuePush(obj->q2, x);
    }
}

// 移除并返回栈顶元素
int myStackPop(MyStack* obj) {
    // 假设 q1 非空，q2 为空 (如果 q1 为空，说明数据全在 q2)
    Queue* non_empty = obj->q1;
    Queue* empty = obj->q2;

    if (queueEmpty(obj->q1)) {
        non_empty = obj->q2;
        empty = obj->q1;
    }

    // 将非空队列的前 size-1 个元素转移到空队列中
    while (queueSize(non_empty) > 1) {
        queuePush(empty, queuePop(non_empty));
    }

    // 此时 non_empty 中只剩最后一个元素，即栈顶元素，将其弹出
    return queuePop(non_empty);
}

// 返回栈顶元素
int myStackTop(MyStack* obj) {
    Queue* non_empty = obj->q1;
    Queue* empty = obj->q2;

    if (queueEmpty(obj->q1)) {
        non_empty = obj->q2;
        empty = obj->q1;
    }

    // 同样转移前 size-1 个元素
    while (queueSize(non_empty) > 1) {
        queuePush(empty, queuePop(non_empty));
    }

    // 获取最后一个元素的值
    int topVal = queuePop(non_empty);
    // 题目要求 top 不能移除元素，所以再把它压回另一个队列（现在它是非空队列了）
    queuePush(empty, topVal);

    return topVal;
}

// 判断栈是否为空
bool myStackEmpty(MyStack* obj) {
    return queueEmpty(obj->q1) && queueEmpty(obj->q2);
}

// 释放内存
void myStackFree(MyStack* obj) {
    queueFree(obj->q1);
    queueFree(obj->q2);
    free(obj);
}

/* ==========================================
 * 3. 测试代码 (Main 函数)
 * ========================================== */
int main() {
    // 按照题目示例进行测试
    printf("初始化栈...\n");
    MyStack* obj = myStackCreate();

    printf("push(1)\n");
    myStackPush(obj, 1);

    printf("push(2)\n");
    myStackPush(obj, 2);

    int param_2 = myStackTop(obj);
    printf("top() -> %d (期望值: 2)\n", param_2);

    int param_3 = myStackPop(obj);
    printf("pop() -> %d (期望值: 2)\n", param_3);

    bool param_4 = myStackEmpty(obj);
    printf("empty() -> %s (期望值: false)\n", param_4 ? "true" : "false");

    int param_5 = myStackPop(obj);
    printf("pop() -> %d (期望值: 1)\n", param_5);

    bool param_6 = myStackEmpty(obj);
    printf("empty() -> %s (期望值: true)\n", param_6 ? "true" : "false");

    // 释放内存
    myStackFree(obj);
    printf("测试结束，内存已释放。\n");

    return 0;
}