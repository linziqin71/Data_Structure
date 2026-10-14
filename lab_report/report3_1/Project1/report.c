#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>

typedef int Status;
typedef int Elemtype;

/* ==================== 栈的结构定义 ==================== */
typedef struct LNode {
    Elemtype data;
    struct LNode* next;
} LNode, * LinkStack;

typedef struct LinkList {
    LinkStack top;   // 栈顶指针
    int length;      // 栈中元素个数
} LinkList;

/* ==================== 队列的结构定义 ==================== */
typedef struct QNode {
    Elemtype data;
    struct QNode* next;
} QNode, * QueuePtr;

typedef struct LinkQueue {
    QueuePtr front;  // 队头指针（指向头结点）
    QueuePtr rear;   // 队尾指针
    int length;      // 队列元素个数
} LinkQueue;

/* ==================== 栈的基本操作 ==================== */

/* 初始化栈 */
Status InitStack(LinkList* s) {
    s->top = NULL;
    s->length = 0;
    return 0;
}

/* 入栈 */
Status Push(LinkList* s, Elemtype e) {
    LinkStack p = (LinkStack)malloc(sizeof(LNode));
    if (!p) return 1;            // 内存分配失败
    p->data = e;
    p->next = s->top;
    s->top = p;
    s->length++;
    return 0;
}

/* 出栈：弹出栈顶元素并通过 *e 返回 */
Status Pop(LinkList* s, Elemtype* e) {
    if (s->top == NULL) return 1; // 空栈
    LinkStack p = s->top;
    *e = p->data;                 // 写回调用者变量
    s->top = p->next;
    s->length--;                  // 同步更新长度
    free(p);
    return 0;
}

/* 打印栈中元素（从栈顶到栈底） */
Status PrintStack(const LinkList* s) {
    if (s->top == NULL) {
        printf("该栈为空栈。\n");
        return 1;
    }
    LinkStack tmp = s->top;
    printf("栈中元素为（栈顶→栈底）：");
    while (tmp != NULL) {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
    return 0;
}

/* ==================== 队列的基本操作 ==================== */

/* 初始化队列：创建头结点 */
Status InitQueue(LinkQueue* Q) {
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
    if (!Q->front) return 1;      // 内存分配失败
    Q->front->next = NULL;
    Q->front->data = 0;           // 头结点数据置零，避免打印垃圾值
    Q->length = 0;
    return 0;
}

/* 入队 */
Status EnQueue(LinkQueue* Q, Elemtype e) {
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if (!p) return 1;
    p->data = e;
    p->next = NULL;
    Q->rear->next = p;
    Q->rear = p;
    Q->length++;
    return 0;
}

/* 出队：弹出队头元素并通过 *e 返回 */
Status DeQueue(LinkQueue* Q, Elemtype* e) {
    if (Q->front == Q->rear) return 1; // 空队
    QueuePtr p = Q->front->next;
    *e = p->data;                      // 写回调用者变量
    Q->front->next = p->next;
    if (Q->rear == p)                  // 若删的是最后一个结点
        Q->rear = Q->front;            // 队尾指回头结点
    Q->length--;                       // 同步更新长度
    free(p);
    return 0;
}

/* 打印队列中元素（从队头到队尾） */
Status PrintQueue(const LinkQueue* Q) {
    if (Q->front == Q->rear) {
        printf("该队为空队。\n");
        return 1;
    }
    QueuePtr tmp = Q->front->next;     // 跳过头结点，从头元素开始
    printf("队中元素为（队头→队尾）：");
    while (tmp != NULL) {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
    return 0;
}

/* ==================== 主函数 ==================== */
int main() {
    Elemtype e = 0;
    LinkList s;
    InitStack(&s);

    LinkQueue Q;
    InitQueue(&Q);

    int choice;
    do {
        printf("****************************\n");
        printf("***********1.入栈***********\n");
        printf("***********2.出栈***********\n");
        printf("***********3.入队***********\n");
        printf("***********4.出队***********\n");
        printf("***********0.退出***********\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // 入栈：8,9,5,4
            printf("8,9,5,4 按顺序入栈...\n");
            Push(&s, 8);
            Push(&s, 9);
            Push(&s, 5);
            Push(&s, 4);
            PrintStack(&s);
            break;

        case 2:
            // 出栈：要弹出 9 和 5，因 LIFO 需先弹出栈顶的 4
            printf("因栈为后进先出，弹出9、5前需先弹出栈顶的4...\n");
            Pop(&s, &e);
            printf("出栈元素：%d\n", e);
            Pop(&s, &e);
            printf("出栈元素：%d\n", e);
            Pop(&s, &e);
            printf("出栈元素：%d\n", e);
            PrintStack(&s);
            break;

        case 3:
            // 入队：4,5,7,6,8
            printf("4,5,7,6,8 按顺序入队...\n");
            EnQueue(&Q, 4);
            EnQueue(&Q, 5);
            EnQueue(&Q, 7);
            EnQueue(&Q, 6);
            EnQueue(&Q, 8);
            PrintQueue(&Q);            // 入队后打印队列
            break;

        case 4:
            // 出队：要弹出 5 和 7，因 FIFO 需先弹出队头的 4
            printf("因队列为先进先出，弹出5、7前需先弹出队头的4...\n");
            DeQueue(&Q, &e);
            printf("出队元素：%d\n", e);
            DeQueue(&Q, &e);
            printf("出队元素：%d\n", e);
            DeQueue(&Q, &e);
            printf("出队元素：%d\n", e);
            PrintQueue(&Q);            //出队后打印队列
            break;

        case 0:
            printf("已退出。\n");
            break;

        default:
            printf("输入值有误，请重新再选择一次。\n");
            break;
        }
    } while (choice);

    return 0;
}