#include <stdio.h>
#include <stdlib.h>

// 1. 定义链表节点结构体
struct ListNode {
    int val;
    struct ListNode* next;
};

// 2. 核心算法：反转链表 
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode* prev = NULL;
    struct ListNode* curr = head;

    while (curr != NULL) {
        struct ListNode* nextTemp = curr->next; // 暂存下一个节点
        curr->next = prev;                      // 反转指针
        prev = curr;                            // prev 前移
        curr = nextTemp;                        // curr 前移
    }

    return prev;
}

// --------- 以下为辅助测试函数 ---------

// 辅助函数：根据数组创建链表，方便测试
struct ListNode* createList(int arr[], int size) {
    if (size == 0) return NULL;

    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;

    struct ListNode* current = head;
    for (int i = 1; i < size; i++) {
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->val = arr[i];
        newNode->next = NULL;
        current->next = newNode;
        current = newNode;
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

// 辅助函数：释放链表内存（防止内存泄漏，C语言好习惯）
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
    // 测试用例：1 -> 2 -> 3 -> 4 -> 5
    int arr[] = { 1, 2, 3, 4, 5 };
    int size = sizeof(arr) / sizeof(arr[0]);

    // 创建原链表
    struct ListNode* head = createList(arr, size);
    printf("原链表: ");
    printList(head);

    // 调用反转函数
    struct ListNode* reversedHead = reverseList(head);

    // 打印反转后的链表
    printf("反转后: ");
    printList(reversedHead);

    // 释放内存
    freeList(reversedHead);

    return 0;
}