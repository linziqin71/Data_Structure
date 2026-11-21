#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 题目给出的二叉树节点结构体定义
struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

// ================= 核心解题代码 =================

bool isUnivalTree(struct TreeNode* root) {
    // 空树也是单值二叉树
    if (root == NULL) {
        return true;
    }

    // 如果左子树存在，且左子节点的值不等于根节点的值，则不是单值二叉树
    if (root->left != NULL && root->left->val != root->val) {
        return false;
    }

    // 如果右子树存在，且右子节点的值不等于根节点的值，则不是单值二叉树
    if (root->right != NULL && root->right->val != root->val) {
        return false;
    }

    // 递归检查左右子树是否也都是单值二叉树
    return isUnivalTree(root->left) && isUnivalTree(root->right);
}

// ================= 辅助函数（用于本地测试） =================

// 创建新节点
struct TreeNode* createNode(int val) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 释放二叉树内存
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    // ---------- 示例 1 测试 ----------
    // 输入: [1,1,1,1,null,1]
    // 期望输出: true
    struct TreeNode* root1 = createNode(1);
    root1->left = createNode(1);
    root1->right = createNode(1);
    root1->left->left = createNode(1);
    root1->left->right = NULL;
    root1->right->left = createNode(1);

    bool result1 = isUnivalTree(root1);
    printf("示例 1 测试结果: %s (期望: true)\n", result1 ? "true" : "false");
    freeTree(root1);

    // ---------- 示例 2 测试 ----------
    // 输入: [2,2,2,5,2]
    // 期望输出: false
    struct TreeNode* root2 = createNode(2);
    root2->left = createNode(2);
    root2->right = createNode(2);
    root2->left->left = createNode(5);
    root2->left->right = createNode(2);

    bool result2 = isUnivalTree(root2);
    printf("示例 2 测试结果: %s (期望: false)\n", result2 ? "true" : "false");
    freeTree(root2);

    return 0;
}