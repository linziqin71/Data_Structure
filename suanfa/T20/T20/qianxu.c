#include <stdio.h>
#include <stdlib.h>

// 1. 定义二叉树节点结构体 (与题目一致)
struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

// 2. 辅助函数：计算二叉树的节点总数
int countNodes(struct TreeNode* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// 3. 辅助函数：递归实现前序遍历 (根 -> 左 -> 右)
void preorder(struct TreeNode* root, int* arr, int* index) {
    if (root == NULL) return;

    // 访问根节点
    arr[(*index)++] = root->val;
    // 遍历左子树
    preorder(root->left, arr, index);
    // 遍历右子树
    preorder(root->right, arr, index);
}

// 4. 核心函数：前序遍历 (LeetCode 144题 官方要求实现的函数)
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* preorderTraversal(struct TreeNode* root, int* returnSize) {
    if (root == NULL) {
        *returnSize = 0;
        return NULL;
    }

    // 先计算节点总数，以便动态分配准确大小的数组
    int totalNodes = countNodes(root);
    int* result = (int*)malloc(totalNodes * sizeof(int));

    int index = 0;
    preorder(root, result, &index);

    *returnSize = totalNodes;
    return result;
}

// 5. 辅助函数：创建新节点 (方便在 main 函数中构建测试用例)
struct TreeNode* createNode(int val) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 6. 辅助函数：释放二叉树内存
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// 7. 主函数：测试
int main() {
    /*
     * 构建示例 1 的二叉树:
     *       1
     *        \
     *         2
     *        /
     *       3
     */
    struct TreeNode* root = createNode(1);
    root->right = createNode(2);
    root->right->left = createNode(3);

    int returnSize = 0;

    // 调用前序遍历函数
    int* result = preorderTraversal(root, &returnSize);

    // 打印结果
    printf("输入: root = [1, null, 2, 3]\n");
    printf("输出: [");
    for (int i = 0; i < returnSize; i++) {
        printf("%d", result[i]);
        if (i < returnSize - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    // 释放内存，防止内存泄漏
    free(result);      // 释放 LeetCode 要求返回的数组
    freeTree(root);    // 释放构建的二叉树

    return 0;
}