#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 4   // 数组长度 = n + 1（假设 n = 3，可用空间 3）

struct Queue {
    int* base;
    int front;
    int rear;
};

/* 初始化队列 */
int InitQueue(struct Queue* queue) {
    queue->base = (int*)malloc(MAXSIZE * sizeof(int));  // 改为 int*
    if (!queue->base) return 1;
    queue->front = queue->rear = 0;
    return 0;
}

/* 入队 */
int push(struct Queue* queue, int e) {
    if ((queue->rear + 1) % MAXSIZE == queue->front) {
        printf("full\n");     // 题目要求只输出 full
        return 1;
    }
    queue->base[queue->rear] = e;
    queue->rear = (queue->rear + 1) % MAXSIZE;
    return 0;
}

/* 取队首，不出队 */
int front(struct Queue* queue, int* e) {
    if (queue->front == queue->rear) {
        printf("empty\n");    //  队空输出 empty
        return 1;             //  必须 return
    }
    *e = queue->base[queue->front];
    printf("%d\n", *e);       //  只输出元素值
    return 0;
}

/* 出队 */
int pop(struct Queue* queue, int* e) {
    if (queue->front == queue->rear) {
        printf("empty\n");    //  队空输出 empty
        return 1;             //  必须 return
    }
    *e = queue->base[queue->front];
    queue->front = (queue->front + 1) % MAXSIZE;
    printf("%d\n", *e);       //  只输出元素值
    return 0;
}

/* 打印队列（调试用，非题目要求） */
void printqueue(struct Queue* queue) {
    printf("当前队列元素为：");
    for (int i = queue->front; i != queue->rear; i = (i + 1) % MAXSIZE) {
        printf("%d ", queue->base[i]);
    }
    printf("\n");
}

int main() {
    struct Queue Q;
    InitQueue(&Q);
    int e = 0;
    int choice = 0;

    do {
        printf("你现在想进行什么操作？\n");
        printf("0.退出 1.push 2.front 3.pop\n");
        scanf("%d", &choice);

        switch (choice) {
        case 0:
            printf("已退出\n");
            break;
        case 1:
            printf("请输入你要放在循环队列中的数（int 类型）：");
            scanf("%d", &e);
            push(&Q, e);
            // printqueue(&Q);   // 调试时可打开
            break;
        case 2:
            front(&Q, &e);
            // printqueue(&Q);
            break;
        case 3:
            pop(&Q, &e);
            // printqueue(&Q);
            break;
        default:
            printf("无效选项\n");
            break;
        }
    } while (choice);

    free(Q.base);   // 释放动态内存
    return 0;
}