
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// 定义二叉树结点
typedef struct Node {
    char data;
    struct Node* left;
    struct Node* right;
} Node;

// 先序建立二叉树，#表示空结点
Node* createTree() {
    char ch;
    scanf(" %c", &ch);

    if (ch == '#')
        return NULL;

    Node* root = (Node*)malloc(sizeof(Node));
    if (root == NULL) {
        printf("内存分配失败！\n");
        exit(1);
    }

    root->data = ch;
    root->left = createTree();
    root->right = createTree();

    return root;
}

// 先序遍历二叉树
void preorder(Node* root) {
    if (root == NULL)
        return;

    printf("%c ", root->data);
    preorder(root->left);
    preorder(root->right);
}

// 交换所有结点的左右子树
void swapTree(Node* root) {
    if (root == NULL)
        return;

    Node* temp = root->left;
    root->left = root->right;
    root->right = temp;

    swapTree(root->left);
    swapTree(root->right);
}

// 释放二叉树内存
void freeTree(Node* root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node* root;

    printf("请输入二叉树的先序序列（#表示空结点）：\n");
    root = createTree();

    printf("交换前的先序遍历：");
    preorder(root);

    swapTree(root);

    printf("\n交换后的先序遍历：");
    preorder(root);

    freeTree(root);

    return 0;
}
