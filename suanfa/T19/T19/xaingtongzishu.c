#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// 1. 定义二叉树节点结构体 (与LeetCode模板一致)
struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

// 2. 辅助函数：判断两棵树是否完全相同
bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
    // 如果两个节点都为空，说明完全匹配
    if (p == NULL && q == NULL) {
        return true;
    }
    // 如果其中一个为空，另一个不为空，说明不匹配
    if (p == NULL || q == NULL) {
        return false;
    }
    // 如果节点值不同，说明不匹配
    if (p->val != q->val) {
        return false;
    }
    // 递归判断左右子树是否完全相同
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

// 3. 核心函数：判断 subRoot 是否是 root 的子树
bool isSubtree(struct TreeNode* root, struct TreeNode* subRoot) {
    // 如果 root 为空，肯定不包含 subRoot
    if (root == NULL) {
        return false;
    }

    // 如果当前 root 节点与 subRoot 是完全相同的树，返回 true
    if (isSameTree(root, subRoot)) {
        return true;
    }

    // 否则，去 root 的左子树或右子树中继续寻找
    return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
}

// ================== 以下为本地测试辅助代码 ==================

// 辅助函数：创建一个新节点
struct TreeNode* createNode(int val) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 主函数，用于本地编译测试
int main() {
    // 构建题目示例中的 root 树: [3, 4, 5, 1, 2]
    struct TreeNode* root = createNode(3);
    root->left = createNode(4);
    root->right = createNode(5);
    root->left->left = createNode(1);
    root->left->right = createNode(2);

    // 构建题目示例中的 subRoot 树: [4, 1, 2]
    struct TreeNode* subRoot = createNode(4);
    subRoot->left = createNode(1);
    subRoot->right = createNode(2);

    // 调用函数并打印结果
    bool result = isSubtree(root, subRoot);
    if (result) {
        printf("输出: true\n");
    }
    else {
        printf("输出: false\n");
    }

    // (注：实际工程中应在这里释放 malloc 分配的内存，此处为了演示简洁省略)

    return 0;
}