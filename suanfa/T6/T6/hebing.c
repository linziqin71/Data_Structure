#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode* next;
};

// 核心算法
struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    struct ListNode* dummy = (struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode* tail = dummy;

    while (list1 != NULL && list2 != NULL) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        }
        else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    if (list1 != NULL) tail->next = list1;
    else tail->next = list2;

    struct ListNode* result = dummy->next;
    free(dummy); // 释放假节点
    return result;
}

// 辅助函数：创建链表
struct ListNode* createList(int arr[], int size) {
    if (size == 0) return NULL;
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;
    struct ListNode* tail = head;
    for (int i = 1; i < size; i++) {
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->val = arr[i];
        newNode->next = NULL;
        tail->next = newNode;
        tail = newNode;
    }
    return head;
}

// 辅助函数：打印链表
void printList(struct ListNode* head) {
    while (head != NULL) {
        printf("%d ", head->val);
        if (head->next != NULL) printf("-> ");
        head = head->next;
    }
    printf("\n");
}

// 辅助函数：释放整条链表
void freeList(struct ListNode* head) {
    while (head != NULL) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    int arr1[] = { 1, 2, 4 };
    int arr2[] = { 1, 3, 4 };

    struct ListNode* list1 = createList(arr1, 3);
    struct ListNode* list2 = createList(arr2, 3);

    printf("原链表1: ");
    printList(list1);
    printf("原链表2: ");
    printList(list2);

    struct ListNode* mergedList = mergeTwoLists(list1, list2);

    printf("合并后: ");
    printList(mergedList);

    // 释放合并后的链表（包含了原本 list1 和 list2 的节点）
    freeList(mergedList);

    return 0;
}