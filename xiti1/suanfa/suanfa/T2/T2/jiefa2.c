#include <stdio.h>
#include <stdlib.h>

// ================= 1. 结构体定义 =================
// LeetCode 提供的结构体定义
struct ListNode {
    int val;
    struct ListNode* next;
};

// ================= 2. 核心解题函数 =================
struct ListNode* removeElements(struct ListNode* head, int val) {
    // 创建虚拟头节点
    struct ListNode* dummy = (struct ListNode*)malloc(sizeof(struct ListNode));
    dummy->val = 0;       // 值随便给，不重要
    dummy->next = head;   // 让虚拟节点指向真实的头节点

    struct ListNode* cur = dummy; // 当前指针从虚拟头节点开始

    // 遍历链表
    while (cur->next != NULL) {
        if (cur->next->val == val) {
            // 找到目标节点
            struct ListNode* tmp = cur->next;
            cur->next = cur->next->next; // 跳过目标节点
            free(tmp); // 释放被删除节点的内存
        }
        else {
            // 不是目标节点，指针后移
            cur = cur->next;
        }
    }

    // 获取新的头节点
    struct ListNode* newHead = dummy->next;

    // 释放虚拟头节点的内存
    free(dummy);

    return newHead;
}

// ================= 3. 辅助函数（方便测试） =================

// 辅助函数：根据数组创建链表
struct ListNode* createList(int* arr, int size) {
    if (size == 0) return NULL;

    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;

    struct ListNode* cur = head;
    for (int i = 1; i < size; i++) {
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->val = arr[i];
        newNode->next = NULL;
        cur->next = newNode;
        cur = cur->next;
    }
    return head;
}

// 辅助函数：打印链表
void printList(struct ListNode* head) {
    printf("[");
    struct ListNode* cur = head;
    while (cur != NULL) {
        printf("%d", cur->val);
        if (cur->next != NULL) {
            printf(", ");
        }
        cur = cur->next;
    }
    printf("]\n");
}

// 辅助函数：释放整个链表的内存（防止内存泄漏）
void freeList(struct ListNode* head) {
    struct ListNode* cur = head;
    while (cur != NULL) {
        struct ListNode* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

// ================= 4. main 函数（测试用例） =================
int main() {
    printf("========== 测试用例 1 ==========\n");
    int arr1[] = { 1, 2, 6, 3, 4, 5, 6 };
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    struct ListNode* list1 = createList(arr1, size1);

    printf("输入: head = ");
    printList(list1);
    printf("删除值: val = 6\n");

    list1 = removeElements(list1, 6); // 执行删除

    printf("输出: ");
    printList(list1);
    freeList(list1); // 释放内存
    printf("\n");



    printf("========== 测试用例 2 ==========\n");
    int arr3[] = { 7, 7, 7, 7 };
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    struct ListNode* list3 = createList(arr3, size3);

    printf("输入: head = ");
    printList(list3);
    printf("删除值: val = 7\n");

    list3 = removeElements(list3, 7);

    printf("输出: ");
    printList(list3);
    freeList(list3);
    printf("\n");

    return 0;
}