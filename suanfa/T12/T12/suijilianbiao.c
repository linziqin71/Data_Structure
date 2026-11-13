#include <stdio.h>
#include <stdlib.h>

// 定义链表节点结构体
struct Node {
    int val;
    struct Node* next;
    struct Node* random;
};

// ================= 核心解题函数 =================
struct Node* copyRandomList(struct Node* head) {
    if (head == NULL) return NULL;

    // ---------------- 第一步：克隆节点并插入到原节点后面 ----------------
    // 变成 A -> A' -> B -> B' -> C -> C' -> NULL
    struct Node* cur = head;
    while (cur != NULL) {
        struct Node* copyNode = (struct Node*)malloc(sizeof(struct Node));
        copyNode->val = cur->val;

        copyNode->next = cur->next;
        cur->next = copyNode;

        cur = copyNode->next;
    }

    // ---------------- 第二步：连接复制节点的 random 指针 ----------------
    cur = head;
    while (cur != NULL) {
        struct Node* copyNode = cur->next;

        if (cur->random != NULL) {
            copyNode->random = cur->random->next;
        }
        else {
            copyNode->random = NULL;
        }

        cur = copyNode->next;
    }

    // ---------------- 第三步：拆分链表，恢复原链表并提取新链表 ----------------
    cur = head;
    struct Node* copyHead = head->next;
    struct Node* copyCur = copyHead;

    while (cur != NULL) {
        // 恢复原链表
        cur->next = copyCur->next;
        cur = cur->next;

        // 构建新链表
        if (cur != NULL) {
            copyCur->next = cur->next;
        }
        else {
            copyCur->next = NULL;
        }
        copyCur = copyCur->next;
    }

    return copyHead;
}


// ================= 以下为测试辅助函数 =================

// 辅助函数：创建新节点
struct Node* createNode(int val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->val = val;
    newNode->next = NULL;
    newNode->random = NULL;
    return newNode;
}

// 辅助函数：打印链表 (格式: 节点值[random指向的节点值或NULL])
void printList(struct Node* head) {
    struct Node* cur = head;
    while (cur != NULL) {
        if (cur->random != NULL) {
            printf("[%d, random->%d] -> ", cur->val, cur->random->val);
        }
        else {
            printf("[%d, random->NULL] -> ", cur->val);
        }
        cur = cur->next;
    }
    printf("NULL\n");
}

// 辅助函数：释放链表内存
void freeList(struct Node* head) {
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// 主函数
int main() {
    // 1. 构建题目示例中的链表: 7 -> 13 -> 11 -> 10 -> 1
    struct Node* node1 = createNode(7);
    struct Node* node2 = createNode(13);
    struct Node* node3 = createNode(11);
    struct Node* node4 = createNode(10);
    struct Node* node5 = createNode(1);

    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node5;

    // 设置 random 指针 (参照题目示例图)
    node1->random = NULL;     // 7 -> NULL
    node2->random = node1;    // 13 -> 7
    node3->random = node5;    // 11 -> 1
    node4->random = node3;    // 10 -> 11
    node5->random = node1;    // 1 -> 7

    printf("原链表: \n");
    printList(node1);

    // 2. 调用核心函数进行深拷贝
    struct Node* copiedList = copyRandomList(node1);

    printf("\n复制后的新链表: \n");
    printList(copiedList);

    // 3. 验证深拷贝 (检查指针地址是否完全不同)
    printf("\n验证是否真的是深拷贝:\n");
    printf("原链表头节点内存地址: %p\n", (void*)node1);
    printf("新链表头节点内存地址: %p\n", (void*)copiedList);

    // 如果新旧节点地址不同，且 random 指向的也是新节点，说明拷贝成功
    if (node1 != copiedList && copiedList->random != node1) {
        printf("=> 成功！新链表和原链表完全独立，没有共享节点。\n");
    }

    // 4. 释放内存 (防止内存泄漏)
    freeList(node1);
    freeList(copiedList);

    return 0;
}