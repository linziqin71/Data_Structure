#include <stdlib.h>

 
 struct ListNode {
    int val;
     struct ListNode *next;
 };

struct ListNode* removeElements(struct ListNode* head, int val) {
    // 1. 处理头节点是需要删除的情况
    // 使用 while 是因为可能有连续的多个头节点都需要删除，例如 [7, 7, 7], val = 7
    while (head != NULL && head->val == val) {
        struct ListNode* tmp = head;
        head = head->next;
        free(tmp); // C 语言需要手动释放被删除节点的内存
    }

    // 2. 处理非头节点
    struct ListNode* cur = head;
    while (cur != NULL && cur->next != NULL) {
        if (cur->next->val == val) {
            struct ListNode* tmp = cur->next;
            cur->next = cur->next->next; // 跳过目标节点
            free(tmp); // 释放内存
        }
        else {
            cur = cur->next; // 不是目标节点，继续往后走
        }
    }

    return head;
}


int main()
{

}