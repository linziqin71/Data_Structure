#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 定义循环队列结构体
typedef struct {
    int* data;      // 存放数据的数组
    int head;       // 队头指针（索引）
    int tail;       // 队尾指针（索引，指向下一个要插入的位置）
    int capacity;   // 队列的实际容量 (k + 1)
} MyCircularQueue;


// 1. 构造函数：设置队列长度为 k
MyCircularQueue* myCircularQueueCreate(int k) {
    MyCircularQueue* obj = (MyCircularQueue*)malloc(sizeof(MyCircularQueue));
    if (obj == NULL) return NULL;

    // 关键点：实际申请 k + 1 的空间，用来区分队空和队满
    obj->capacity = k + 1;
    obj->data = (int*)malloc(sizeof(int) * obj->capacity);
    obj->head = 0;
    obj->tail = 0;

    return obj;
}

// 2. 检查循环队列是否为空
bool myCircularQueueIsEmpty(MyCircularQueue* obj) {
    return obj->head == obj->tail;
}

// 3. 检查循环队列是否已满
bool myCircularQueueIsFull(MyCircularQueue* obj) {
    // 如果 tail 的下一个位置是 head，则说明满了
    return (obj->tail + 1) % obj->capacity == obj->head;
}

// 4. 向循环队列插入一个元素。成功返回 true
bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {
    if (myCircularQueueIsFull(obj)) {
        return false; // 队列已满，插入失败
    }

    obj->data[obj->tail] = value; // 在 tail 处插入
    obj->tail = (obj->tail + 1) % obj->capacity; // tail 往后移，并取模实现循环

    return true;
}

// 5. 从循环队列中删除一个元素。成功返回 true
bool myCircularQueueDeQueue(MyCircularQueue* obj) {
    if (myCircularQueueIsEmpty(obj)) {
        return false; // 队列为空，删除失败
    }

    // 只需要把 head 往后移动一位即可，不需要实际清除数据
    obj->head = (obj->head + 1) % obj->capacity;

    return true;
}

// 6. 从队首获取元素。如果队列为空，返回 -1
int myCircularQueueFront(MyCircularQueue* obj) {
    if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }
    return obj->data[obj->head];
}

// 7. 获取队尾元素。如果队列为空，返回 -1
int myCircularQueueRear(MyCircularQueue* obj) {
    if (myCircularQueueIsEmpty(obj)) {
        return -1;
    }
    // 注意：tail 指向的是下一个插入位置，所以真实的队尾元素在 tail 的前一个位置。
    // 为了防止越界（tail 可能为 0），需要加上 capacity 再取模
    return obj->data[(obj->tail - 1 + obj->capacity) % obj->capacity];
}

// 8. 释放内存
void myCircularQueueFree(MyCircularQueue* obj) {
    free(obj->data);
    free(obj);
}

// ==========================================
// 下面是用于测试的 main 函数
// ==========================================
int main() {
    // 初始化一个容量为 3 的循环队列
    MyCircularQueue* circularQueue = myCircularQueueCreate(3);

    // 测试入队
    printf("enQueue(1): %s\n", myCircularQueueEnQueue(circularQueue, 1) ? "true" : "false"); // 返回 true
    printf("enQueue(2): %s\n", myCircularQueueEnQueue(circularQueue, 2) ? "true" : "false"); // 返回 true
    printf("enQueue(3): %s\n", myCircularQueueEnQueue(circularQueue, 3) ? "true" : "false"); // 返回 true

    // 测试队满时入队
    printf("enQueue(4): %s\n", myCircularQueueEnQueue(circularQueue, 4) ? "true" : "false"); // 返回 false，队列已满

    // 获取队尾元素
    printf("Rear(): %d\n", myCircularQueueRear(circularQueue)); // 返回 3

    // 测试是否已满
    printf("isFull(): %s\n", myCircularQueueIsFull(circularQueue) ? "true" : "false"); // 返回 true

    // 测试出队
    printf("deQueue(): %s\n", myCircularQueueDeQueue(circularQueue) ? "true" : "false"); // 返回 true

    // 测试出队后入队（验证循环特性）
    printf("enQueue(4): %s\n", myCircularQueueEnQueue(circularQueue, 4) ? "true" : "false"); // 返回 true

    // 获取队尾元素
    printf("Rear(): %d\n", myCircularQueueRear(circularQueue)); // 返回 4

    // 获取队首元素
    printf("Front(): %d\n", myCircularQueueFront(circularQueue)); // 返回 2

    // 释放内存
    myCircularQueueFree(circularQueue);

    return 0;
}