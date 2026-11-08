#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 链表节点结构体
struct ListNode {
    int val;
    struct ListNode* next;
};

/**
 * 核心解题函数：快慢指针（Floyd 判圈算法）
 */
bool hasCycle(struct ListNode* head) {
    // 边界情况：空链表或只有一个节点且无环，直接返回 false
    if (head == NULL || head->next == NULL) {
        return false;
    }

    // 初始化快慢指针，同一起点出发
    struct ListNode* slow = head;
    struct ListNode* fast = head;

    // 只要快指针和它的下一步都不为空，就继续跑
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;           // 慢指针每次走 1 步
        fast = fast->next->next;     // 快指针每次走 2 步

        // 如果两者相遇，说明套圈了，存在环！
        if (slow == fast) {
            return true;
        }
    }

    // 跳出循环说明快指针走到了尽头（遇到了 NULL），说明无环
    return false;
}

// ----------------- 以下为测试代码 -----------------

struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

int main() {
    // 1. 构建有环链表：3 -> 2 -> 0 -> -4
    //                         ^         |
    //                         |_________|
    struct ListNode* n1 = createNode(3);
    struct ListNode* n2 = createNode(2);
    struct ListNode* n3 = createNode(0);
    struct ListNode* n4 = createNode(-4);

    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n2; // 关键：将尾部 -4 连回节点 2，制造环 (对应题目 pos=1)

    // 2. 调用函数
    bool result1 = hasCycle(n1);
    printf("测试1 (有环链表): %s\n", result1 ? "true (有环)" : "false (无环)");

    // 3. 再构建一个无环链表测试：1 -> 2 -> 3
    struct ListNode* a1 = createNode(1);
    struct ListNode* a2 = createNode(2);
    struct ListNode* a3 = createNode(3);
    a1->next = a2;
    a2->next = a3;
    a3->next = NULL;

    bool result2 = hasCycle(a1);
    printf("测试2 (无环链表): %s\n", result2 ? "true (有环)" : "false (无环)");

    // 注意：因为测试用例1中有环，直接 free 会导致死循环。
    // 此处仅为演示代码，省略内存释放步骤。

    return 0;
}