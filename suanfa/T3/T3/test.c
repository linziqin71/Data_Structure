#include <stdio.h>

// ==========================================
// 核心算法函数：删除有序数组中的重复项
// ==========================================
int removeDuplicates(int* nums, int numsSize) {
    // 边界条件：如果数组为空，直接返回 0
    if (numsSize == 0) {
        return 0;
    }

    // 慢指针，记录已去重部分的最后一个元素的下标
    int slow = 0;

    // 快指针，用于遍历整个数组寻找新元素
    for (int fast = 1; fast < numsSize; fast++) {
        // 如果发现新元素（与慢指针指向的元素不同）
        if (nums[fast] != nums[slow]) {
            slow++; // 慢指针前移，腾出位置
            nums[slow] = nums[fast]; // 将新元素覆盖到慢指针位置
        }
    }

    // 返回新数组的长度（下标 + 1）
    return slow + 1;
}

// ==========================================
// 辅助函数：打印数组元素（用于验证结果）
// ==========================================
void printArray(int* nums, int length) {
    printf("[");
    for (int i = 0; i < length; i++) {
        printf("%d", nums[i]);
        if (i < length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// ==========================================
// 主函数：执行测试用例
// ==========================================
int main() {
    printf("========== 测试用例 1：包含 0 ==========\n");
    // 定义包含 0 的有序数组
    int nums1[] = { 0, 0, 1, 1, 1, 2, 2, 3, 3, 4 };
    int size1 = sizeof(nums1) / sizeof(nums1[0]); // 计算数组长度

    printf("原数组: ");
    printArray(nums1, size1);

    // 调用去重函数
    int newLength1 = removeDuplicates(nums1, size1);

    printf("去重后长度: %d\n", newLength1);
    printf("去重后数组的前 %d 个元素: ", newLength1);
    printArray(nums1, newLength1); // 只打印有效部分
    printf("\n");


    printf("========== 测试用例 2：不包含 0 ==========\n");
    // 定义不包含 0 的有序数组
    int nums2[] = { -3, -1, -1, 0, 0, 2 };
    // 注意：为了满足“不含0”，我们将数组改为全负数或正数，这里用负数示例
    int nums2_no_zero[] = { -3, -3, -1, 2, 2, 2, 5 };
    int size2 = sizeof(nums2_no_zero) / sizeof(nums2_no_zero[0]);

    printf("原数组: ");
    printArray(nums2_no_zero, size2);

    // 调用去重函数
    int newLength2 = removeDuplicates(nums2_no_zero, size2);

    printf("去重后长度: %d\n", newLength2);
    printf("去重后数组的前 %d 个元素: ", newLength2);
    printArray(nums2_no_zero, newLength2);
    printf("\n");

    return 0;
}