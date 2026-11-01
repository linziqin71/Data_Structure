#include <stdio.h>
#include <stdlib.h>

// 1. 定义二叉树的节点结构体
typedef int BTDataType;
typedef struct BinaryTreeNode {
    BTDataType data;                // 数据域
    struct BinaryTreeNode* left;    // 左孩子指针
    struct BinaryTreeNode* right;   // 右孩子指针
} BTNode;

// 辅助函数：创建一个新节点
BTNode* BuyNode(BTDataType x) {
    BTNode* node = (BTNode*)malloc(sizeof(BTNode));
    if (node == NULL) {
        perror("malloc fail");
        exit(-1);
    }
    node->data = x;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 2. 手动构建一棵二叉树
//       1
//      / \
//     2   4
//    /   / \
//   3   5   6
BTNode* CreateBinaryTree() {
    BTNode* node1 = BuyNode(1);
    BTNode* node2 = BuyNode(2);
    BTNode* node3 = BuyNode(3);
    BTNode* node4 = BuyNode(4);
    BTNode* node5 = BuyNode(5);
    BTNode* node6 = BuyNode(6);

    // 把节点连接起来
    node1->left = node2;
    node1->right = node4;
    node2->left = node3;
    node4->left = node5;
    node4->right = node6;

    return node1; // 返回根节点
}

// 3. 前序遍历的核心代码
void PrevOrder(BTNode* root) {
    // 如果节点为空，打印 N 并返回
    if (root == NULL) {
        printf("N ");
        return;
    }

    // 按照 根 -> 左子树 -> 右子树 的顺序
    printf("%d ", root->data);
    PrevOrder(root->left);
    PrevOrder(root->right);

    // 这里隐含了 return; （函数自然结束）
}

// 4. 主函数
int main() {
    // 第一步：建树，拿到根节点
    BTNode* root = CreateBinaryTree();

    printf("前序遍历结果：\n");

    // 第二步：调用遍历函数
    PrevOrder(root);

    printf("\n(注意：最后的 N 会很多，是因为最底层的叶子节点左右两边都是空)\n");

    return 0;
}