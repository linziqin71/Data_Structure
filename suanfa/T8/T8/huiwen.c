#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 定义链表节点结构体
struct ListNode {
    int val;
    struct ListNode* next;
};

// ---------------------------------------------------------
// 辅助函数：反转链表 
// ---------------------------------------------------------
struct ListNode* reverseList(struct ListNode* head) {
    struct ListNode* prev = NULL;
    struct ListNode* curr = head;
    while (curr != NULL) {
        struct ListNode* nextTemp = curr->next; // 暂存下一个节点
        curr->next = prev;                      // 当前节点指向前一个节点
        prev = curr;                            // prev 前进
        curr = nextTemp;                        // curr 前进
    }
    return prev; // 返回新的头节点
}

// ---------------------------------------------------------
// 核心函数：判断回文链表 
// ---------------------------------------------------------
bool chkPalindrome(struct ListNode* A) {
    if (A == NULL || A->next == NULL) {
        return true; // 空链表或只有一个节点，肯定是回文
    }

    // 1. 快慢指针找中点
    struct ListNode* slow = A;
    struct ListNode* fast = A->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;       // 慢指针走一步
        fast = fast->next->next; // 快指针走两步
    }

    // 此时 slow 就是前半段的最后一个节点

    // 2. 反转后半部分链表
    struct ListNode* secondHalfStart = reverseList(slow->next);

    // 3. 准备两个指针进行比对
    struct ListNode* p1 = A;                 // 左边开头
    struct ListNode* p2 = secondHalfStart;   // 右边开头(反转后的开头)
    bool result = true;                      // 记录结果

    while (p2 != NULL) {
        if (p1->val != p2->val) {
            result = false; // 发现不一致，不是回文
            break;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    // 4. 恢复链表结构（将后半段再反转回来）
    slow->next = reverseList(secondHalfStart);

    return result;
}

// ---------------------------------------------------------
// 以下是为了测试写的辅助函数和 main 函数
// ---------------------------------------------------------

// 辅助函数：根据数组创建链表
struct ListNode* createList(int* arr, int size) {
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
        current = current->next;
    }
    return head;
}

// 辅助函数：打印链表（用于验证是否恢复了结构）
void printList(struct ListNode* head) {
    struct ListNode* curr = head;
    while (curr != NULL) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

// 主函数
int main() {
    // 测试用例 1：偶数个节点，是回文
    int arr1[] = { 1, 2, 2, 1 };
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    struct ListNode* list1 = createList(arr1, size1);

    printf("测试用例 1: ");
    printList(list1);
    if (chkPalindrome(list1)) {
        printf("结果: true (是回文)\n");
    }
    else {
        printf("结果: false (不是回文)\n");
    }
    // 验证链表结构是否被恢复
    printf("比对完成后恢复的链表: ");
    printList(list1);
    printf("-----------------------------------\n");

    // 测试用例 2：奇数个节点，是回文
    int arr2[] = { 1, 2, 3, 2, 1 };
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    struct ListNode* list2 = createList(arr2, size2);

    printf("测试用例 2: ");
    printList(list2);
    if (chkPalindrome(list2)) {
        printf("结果: true (是回文)\n");
    }
    else {
        printf("结果: false (不是回文)\n");
    }
    printf("比对完成后恢复的链表: ");
    printList(list2);
    printf("-----------------------------------\n");

    // 测试用例 3：不是回文
    int arr3[] = { 1, 2, 3, 4 };
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    struct ListNode* list3 = createList(arr3, size3);

    printf("测试用例 3: ");
    printList(list3);
    if (chkPalindrome(list3)) {
        printf("结果: true (是回文)\n");
    }
    else {
        printf("结果: false (不是回文)\n");
    }
    printf("-----------------------------------\n");

    return 0;
}