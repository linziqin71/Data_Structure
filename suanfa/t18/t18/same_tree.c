#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 定义二叉树节点结构体
struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

// --- 核心解题函数 ---
bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
    // 1. 如果两个节点都为空，认为相同
    if (p == NULL && q == NULL) {
        return true;
    }
    // 2. 如果其中一个为空，另一个不为空，结构不同，返回 false
    if (p == NULL || q == NULL) {
        return false;
    }
    // 3. 如果两个节点都不为空，但值不同，返回 false
    if (p->val != q->val) {
        return false;
    }
    // 4. 递归比较左子树和右子树
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

// --- 辅助函数：创建新节点 ---
struct TreeNode* createNode(int val) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// --- 辅助函数：释放二叉树内存 ---
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    // 测试用例 1 (对应示例 1)
    // 树 p: [1, 2, 3]
    struct TreeNode* p1 = createNode(1);
    p1->left = createNode(2);
    p1->right = createNode(3);

    // 树 q: [1, 2, 3]
    struct TreeNode* q1 = createNode(1);
    q1->left = createNode(2);
    q1->right = createNode(3);

    printf("测试用例 1:\n");
    if (isSameTree(p1, q1)) {
        printf("结果: true (两棵树相同)\n");
    }
    else {
        printf("结果: false (两棵树不同)\n");
    }

    // 测试用例 2 (对应示例 2)
    // 树 p: [1, 2]
    struct TreeNode* p2 = createNode(1);
    p2->left = createNode(2);

    // 树 q: [1, null, 2]
    struct TreeNode* q2 = createNode(1);
    q2->right = createNode(2);

    printf("\n测试用例 2:\n");
    if (isSameTree(p2, q2)) {
        printf("结果: true (两棵树相同)\n");
    }
    else {
        printf("结果: false (两棵树不同)\n");
    }

    // 释放内存
    freeTree(p1);
    freeTree(q1);
    freeTree(p2);
    freeTree(q2);

    return 0;
}