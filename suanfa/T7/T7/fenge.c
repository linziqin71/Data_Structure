#include <stdio.h>
#include <stdlib.h>

// 1. 定义链表节点结构体
struct ListNode {
    int val;
    struct ListNode* next;
};

// ================= 核心算法：链表分割 =================
struct ListNode* partition(struct ListNode* pHead, int x) {
    // 准备两个虚拟头节点 (纸箱 A 和 纸箱 B)
    struct ListNode* lessDummy = (struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode* lessTail = lessDummy;

    struct ListNode* greaterDummy = (struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode* greaterTail = greaterDummy;

    // 遍历原链表开始分拣
    struct ListNode* curr = pHead;
    while (curr != NULL) {
        if (curr->val < x) {
            lessTail->next = curr;
            lessTail = lessTail->next;
        }
        else {
            greaterTail->next = curr;
            greaterTail = greaterTail->next;
        }
        curr = curr->next;
    }

    // 拼接两个纸箱
    lessTail->next = greaterDummy->next;

    // 【极度重要】把纸箱 B 的尾巴斩断，防止出现环形死循环
    greaterTail->next = NULL;

    // 记录结果并释放假底内存
    struct ListNode* result = lessDummy->next;
    free(lessDummy);
    free(greaterDummy);

    return result;
}

// ================= 辅助函数：创建链表 =================
// 使用尾插法，保证链表顺序和数组顺序一致
struct ListNode* createList(int arr[], int size) {
    if (size == 0) return NULL;
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;
    struct ListNode* tail = head;

    for (int i = 1; i < size; i++) {
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->val = arr[i];
        newNode->next = NULL;
        tail->next = newNode;
        tail = newNode;
    }
    return head;
}

// ================= 辅助函数：打印链表 =================
void printList(struct ListNode* head) {
    while (head != NULL) {
        printf("%d ", head->val);
        if (head->next != NULL) printf("-> ");
        head = head->next;
    }
    printf("\n");
}

// ================= 辅助函数：释放整条链表 =================
void freeList(struct ListNode* head) {
    while (head != NULL) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

// ================= 主函数 =================
int main() {
    // 测试用例：原链表 1 -> 4 -> 3 -> 2 -> 5 -> 2， 给定 x = 3
    int arr[] = { 1, 4, 3, 2, 5, 2 };
    int size = sizeof(arr) / sizeof(arr[0]);
    int x = 3;

    // 1. 创建并打印原链表
    struct ListNode* head = createList(arr, size);
    printf("原链表: ");
    printList(head);

    // 2. 执行分割算法
    printf("分割基准 x = %d\n", x);
    struct ListNode* newHead = partition(head, x);

    // 3. 打印分割后的链表
    printf("分割后: ");
    printList(newHead);

    // 4. 释放内存
    freeList(newHead);

    return 0;
}