#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode* next;
};

// 快指针走 3 步的版本
bool hasCycleFast3(struct ListNode* head) {
    if (head == NULL || head->next == NULL) return false;

    struct ListNode* slow = head;
    struct ListNode* fast = head;

    // 必须确保 fast 连走 3 步都不会遇到 NULL
    while (fast != NULL && fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;                       // 走 1 步
        fast = fast->next->next->next;           // 走 3 步

        if (slow == fast) {
            return true;
        }
    }

    return false;
}

// 辅助函数
struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

int main() {
    // 构造无环链表: 1 -> 2 -> 3 -> 4
    struct ListNode* n1 = createNode(1);
    struct ListNode* n2 = createNode(2);
    struct ListNode* n3 = createNode(3);
    struct ListNode* n4 = createNode(4);
    n1->next = n2; n2->next = n3; n3->next = n4;

    printf("无环链表测试: %s\n", hasCycleFast3(n1) ? "有环" : "无环");

    // 构造有环链表: 1 -> 2 -> 3 -> 4 -> (回到) 2
    n4->next = n2;

    printf("有环链表测试: %s\n", hasCycleFast3(n1) ? "有环" : "无环");

    return 0;
}