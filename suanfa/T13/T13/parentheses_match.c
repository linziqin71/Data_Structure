#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // malloc 和 free 需要这个头文件

bool isValid(char* s) {
    int len = strlen(s);

    // 1. 奇数长度直接返回 false (没有申请内存，直接返回没问题)
    if (len % 2 != 0) {
        return false;
    }

    // 2. 动态申请内存来模拟栈
    // 申请 len + 1 个字节，+1 是个好习惯，防止意外越界，同时能放下字符串结束符 '\0'（虽然这里用不到）
    char* stack = (char*)malloc(sizeof(char) * (len + 1));
    if (stack == NULL) {
        // 如果内存分配失败（极少情况），直接返回 false
        return false;
    }

    int top = 0;
    bool isValidResult = true; // 默认结果有效，如果中途发现不匹配就改成 false

    // 3. 遍历字符串
    for (int i = 0; i < len; i++) {
        char c = s[i];

        // 如果是左括号，入栈
        if (c == '(' || c == '[' || c == '{') {
            stack[top] = c;
            top++;
        }
        // 如果是右括号
        else {
            // 如果栈是空的，说明右括号多了，不匹配
            if (top == 0) {
                isValidResult = false;
                break; // 发现错误，跳出循环
            }

            char topChar = stack[top - 1];

            // 检查是否匹配
            if ((c == ')' && topChar != '(') ||
                (c == ']' && topChar != '[') ||
                (c == '}' && topChar != '{')) {
                isValidResult = false;
                break; // 发现错误，跳出循环
            }

            // 匹配成功，出栈
            top--;
        }
    }

    // 4. 遍历结束后，如果栈不是空的（左括号多了），也是无效的
    if (top != 0) {
        isValidResult = false;
    }

    // 5. 【极其重要】释放动态申请的内存！防止内存泄漏
    free(stack);

    // 返回最终结果
    return isValidResult;
}

// 主函数，用于测试
int main() {
    char* test1 = "()";
    char* test2 = "()[]{}";
    char* test3 = "(]";
    char* test4 = "([)]";
    char* test5 = "{[]}";
    char* test6 = "(";   // 测试左括号多的情况
    char* test7 = ")";   // 测试右括号多的情况

    printf("测试 1: \"%s\" -> %s\n", test1, isValid(test1) ? "true" : "false");
    printf("测试 2: \"%s\" -> %s\n", test2, isValid(test2) ? "true" : "false");
    printf("测试 3: \"%s\" -> %s\n", test3, isValid(test3) ? "true" : "false");
    printf("测试 4: \"%s\" -> %s\n", test4, isValid(test4) ? "true" : "false");
    printf("测试 5: \"%s\" -> %s\n", test5, isValid(test5) ? "true" : "false");
    printf("测试 6: \"%s\" -> %s\n", test6, isValid(test6) ? "true" : "false");
    printf("测试 7: \"%s\" -> %s\n", test7, isValid(test7) ? "true" : "false");

    return 0;
}