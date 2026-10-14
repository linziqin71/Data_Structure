#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define STACK_INT_SIZE 2   // 车库最大车位数量 n（题目测试数据 n=2）
#define MAX_CAR_SIZE 100   // 数组最大容量保护

/* ==================== 车辆结构体 ==================== */
struct Car {
    char choice;  // 'A' 到达 / 'D' 离开
    int  car_num; // 车牌号
    int  time;    // 到达或离开的时间
};

/* ==================== 顺序栈：模拟停车场 / 临时栈 ==================== */
struct Stack {
    struct Car cars[MAX_CAR_SIZE];
    int top;  // 栈顶指针（指向下一个空位）
};

/* ==================== 链队列：模拟便道 ==================== */
struct QNode {
    struct Car car;
    struct QNode* next;
};

struct Queue {
    struct QNode* front;
    struct QNode* rear;
    int queuesize;
};

/* ==================== 栈的操作 ==================== */

/* 初始化栈 */
void InitStack(struct Stack* stack) {
    stack->top = 0;
}

/* 判断栈空 */
int StackEmpty(struct Stack* stack) {
    return stack->top == 0;
}

/* 入栈：停车 */
void Push(struct Stack* stack, struct Car car) {
    if (stack->top >= MAX_CAR_SIZE) {          // ★ 容量保护
        printf("栈已满，无法停车！\n");
        return;
    }
    stack->cars[stack->top] = car;
    stack->top++;
}

/* 出栈：返回栈顶元素 */
struct Car Pop(struct Stack* stack) {
    struct Car c = stack->cars[--stack->top];
    return c;
}

/* ==================== 队列的操作 ==================== */

/* 初始化队列 */
void InitQueue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->queuesize = 0;
}

/* 判断队空 */
int QueueEmpty(struct Queue* queue) {
    return queue->front == NULL;
}

/* 入队：进入便道排队 */
void EnQueue(struct Queue* queue, struct Car car) {
    struct QNode* new_node = (struct QNode*)malloc(sizeof(struct QNode));
    if (new_node == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    new_node->car = car;
    new_node->next = NULL;

    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    }
    else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->queuesize++;
    printf("车牌 %d 已进入便道排队，当前排在第 %d 位。\n",
        car.car_num, queue->queuesize);
}

/* 出队：便道第一辆车离开队列 */
int DeQueue(struct Queue* queue, struct Car* car) {
    if (QueueEmpty(queue)) return 0;   // 队空
    struct QNode* p = queue->front;
    *car = p->car;
    queue->front = p->next;
    if (queue->front == NULL) queue->rear = NULL;
    free(p);
    queue->queuesize--;
    return 1;
}

/* ==================== 功能函数 ==================== */

/* 打印停车场和便道的当前状态（便于观察实验结果） */
void ShowStatus(struct Stack* park, struct Queue* queue) {
    printf("------------ 当前状态 ------------\n");
    printf("停车场内车辆（从内到外）：");
    if (StackEmpty(park)) {
        printf("空\n");
    }
    else {
        for (int i = 0; i < park->top; i++) {
            printf("%d ", park->cars[i].car_num);
        }
        printf("\n");
    }

    printf("便道等待车辆（从前往后）：");
    if (QueueEmpty(queue)) {
        printf("空\n");
    }
    else {
        struct QNode* p = queue->front;
        while (p != NULL) {
            printf("%d ", p->car.car_num);
            p = p->next;
        }
        printf("\n");
    }
    printf("----------------------------------\n");
}

/* 车辆到达 */
void Arrive(struct Stack* park, struct Queue* queue, struct Car car) {
    if (park->top < STACK_INT_SIZE) {
        // 停车场未满，直接停车
        Push(park, car);
        printf("车牌 %d 停车成功，停在停车场第 %d 位（从内向外数）。\n",
            car.car_num, park->top);
    }
    else {
        // 停车场已满，进入便道排队
        printf("目前车位已满，请车牌 %d 在便道上等候。\n", car.car_num);
        EnQueue(queue, &car == NULL ? car : car); // 直接传值
    }
}

/* 车辆离开 */
void Departure(struct Stack* park, struct Stack* tmp_park,
    struct Queue* queue, struct Car* car) {
    int found = 0;

    /* 1. 从栈顶开始找目标车，把挡路的车暂存到临时栈 */
    while (!StackEmpty(park)) {
        struct Car top_car = Pop(park);
        if (top_car.car_num == car->car_num) {
            found = 1;
            int parking_time = car->time - top_car.time;
            if (parking_time < 0) parking_time = 0;
            int charge = parking_time * 30;   // 30 元/小时
            printf("车牌 %d 离开，停车时长 %d 小时，应缴费用 %d 元。\n",
                top_car.car_num, parking_time, charge);
            break;
        }
        else {
            Push(tmp_park, top_car);
        }
    }

    /* 2. 把临时栈的车按原顺序停回停车场 */
    while (!StackEmpty(tmp_park)) {
        struct Car c = Pop(tmp_park);
        Push(park, c);
    }

    if (!found) {
        printf("没有找到车牌号为 %d 的车辆。\n", car->car_num);
        return;
    }

    /* 3. 便道若有车，则第一辆车进入停车场 */
    if (!QueueEmpty(queue)) {
        struct Car enter_car;
        if (DeQueue(queue, &enter_car)) {
            enter_car.time = car->time;    // 便道等待不收费：以当前时刻作为进入停车场时间
            Push(park, enter_car);
            printf("便道上的车牌 %d 进入停车场（进入时间记为 %d）。\n",
                enter_car.car_num, enter_car.time);
        }
    }
}

/* ==================== 主函数 ==================== */
int main() {
    char choice;
    int  car_num = 0;
    int  time = 0;

    struct Stack park, tmp_park;
    struct Queue queue;

    InitStack(&park);
    InitStack(&tmp_park);
    InitQueue(&queue);

    printf("停车场收费标准：每小时 30 元\n");
    printf("请输入车辆信息：（A）到达  （D）离去  （E）退出\n");

    while (1) {
        printf("\n请选择操作：");
        scanf(" %c", &choice);          // 前面加空格，自动跳过回车/空白

        switch (choice) {
        case 'A': case 'a': {
            printf("请输入车牌号：");
            scanf("%d", &car_num);
            printf("请输入到达时间（单位：小时）：");
            scanf("%d", &time);

            if (time < 0) {
                printf("时间无效，请重新输入。\n");
                break;
            }
            struct Car car = { 'A', car_num, time };
            Arrive(&park, &queue, car);
            ShowStatus(&park, &queue);
            break;
        }

        case 'D': case 'd': {
            printf("请输入车牌号：");
            scanf("%d", &car_num);
            printf("请输入离开时间（单位：小时）：");
            scanf("%d", &time);

            if (time < 0) {
                printf("时间无效，请重新输入。\n");
                break;
            }
            struct Car departure_car = { 'D', car_num, time };
            Departure(&park, &tmp_park, &queue, &departure_car);
            ShowStatus(&park, &queue);
            break;
        }

        case 'E': case 'e':
            printf("已退出。\n");
            return 0;                   // 直接结束程序

        default:
            printf("无效选项，请重新输入。\n");
            break;
        }
    }

    return 0;
}