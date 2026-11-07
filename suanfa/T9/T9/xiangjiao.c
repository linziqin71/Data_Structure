#include <stdio.h>
#include <stdlib.h>

// 链表节点结构体定义
struct ListNode {
    int val;
    struct ListNode* next;
};

/**
 * 核心解题函数
 */
struct ListNode* getIntersectionNode(struct ListNode* headA, struct ListNode* headB) {
    if (headA == NULL || headB == NULL) {
        return NULL;
    }

    struct ListNode* pA = headA;
    struct ListNode* pB = headB;

    // 当 pA 和 pB 不相等时，继续遍历
    // 如果它们相交，会在相交节点处相等而退出循环
    // 如果它们不相交，最终会同时变成 NULL 而退出循环
    while (pA != pB) {
        // pA 如果走到了 A 的末尾，就跳到 B 的开头；否则继续往下走
        pA = (pA == NULL) ? headB : pA->next;

        // pB 如果走到了 B 的末尾，就跳到 A 的开头；否则继续往下走
        pB = (pB == NULL) ? headA : pB->next;
    }

    return pA; // 返回交点（或者 NULL）
}

// ----------------- 以下为测试代码 -----------------

// 辅助函数：创建新节点
struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

int main() {
    // 1. 构建公共相交部分 (c1 -> c2 -> c3)
    struct ListNode* c1 = createNode(8);
    struct ListNode* c2 = createNode(4);
    struct ListNode* c3 = createNode(5);
    c1->next = c2;
    c2->next = c3;

    // 2. 构建链表 A (a1 -> a2 -> c1)
    struct ListNode* a1 = createNode(4);
    struct ListNode* a2 = createNode(1);
    a1->next = a2;
    a2->next = c1; // 连接到公共部分

    // 3. 构建链表 B (b1 -> b2 -> b3 -> c1)
    struct ListNode* b1 = createNode(5);
    struct ListNode* b2 = createNode(6);
    struct ListNode* b3 = createNode(1);
    b1->next = b2;
    b2->next = b3;
    b3->next = c1; // 连接到公共部分

    // 4. 调用函数
    struct ListNode* result = getIntersectionNode(a1, b1);

    // 5. 打印结果
    if (result != NULL) {
        printf("相交节点找到，其值为: %d\n", result->val);
    }
    else {
        printf("两个链表不相交 (返回 NULL)\n");
    }

    // 内存释放（略，实际工程中需释放所有节点内存）

    return 0;
}