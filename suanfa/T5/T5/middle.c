#include <stdio.h>
#include <stdlib.h>

// 1. 定义链表节点结构体
struct ListNode {
    int val;
    struct ListNode* next;
};

// 2. 核心算法：寻找中间节点（快慢指针法）
struct ListNode* middleNode(struct ListNode* head) {
    struct ListNode* slow = head;
    struct ListNode* fast = head;

    // 快指针走两步，慢指针走一步
    // 必须同时保证 fast 和 fast->next 不为空，防止越界崩溃
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

// --------- 以下为辅助测试函数 ---------

// 辅助函数：根据数组创建链表（尾插法，保持原数组顺序）
struct ListNode* createList(int arr[], int size) {
    if (size == 0) return NULL;

    // 创建头节点
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;

    struct ListNode* tail = head; // 尾指针，用于在尾部插入新节点

    for (int i = 1; i < size; i++) {
        // 每来一个新数据，就 malloc 一个新节点
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->val = arr[i];
        newNode->next = NULL;

        tail->next = newNode; // 把新节点挂在尾部
        tail = newNode;       // 尾指针移动到新的尾部
    }
    return head;
}

// 辅助函数：打印链表
void printList(struct ListNode* head) {
    struct ListNode* curr = head;
    while (curr != NULL) {
        printf("%d ", curr->val);
        if (curr->next != NULL) {
            printf("-> ");
        }
        curr = curr->next;
    }
    printf("\n");
}

// 辅助函数：释放链表内存
void freeList(struct ListNode* head) {
    struct ListNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// 3. 主函数
int main() {
    // --------- 测试用例 1：奇数个节点 ---------
    printf("--- 测试用例 1 (奇数个节点) ---\n");
    int arr1[] = { 1, 2, 3, 4, 5 };
    int size1 = sizeof(arr1) / sizeof(arr1[0]);

    struct ListNode* head1 = createList(arr1, size1);
    printf("原链表: ");
    printList(head1);

    struct ListNode* mid1 = middleNode(head1);
    printf("中间节点及之后的链表: ");
    printList(mid1);

    freeList(head1); // 释放整条链表
    printf("\n");


    // --------- 测试用例 2：偶数个节点 ---------
    printf("--- 测试用例 2 (偶数个节点) ---\n");
    int arr2[] = { 1, 2, 3, 4, 5, 6 };
    int size2 = sizeof(arr2) / sizeof(arr2[0]);

    struct ListNode* head2 = createList(arr2, size2);
    printf("原链表: ");
    printList(head2);

    struct ListNode* mid2 = middleNode(head2);
    printf("中间节点及之后的链表: ");
    printList(mid2);

    freeList(head2); // 释放整条链表

    return 0;
}