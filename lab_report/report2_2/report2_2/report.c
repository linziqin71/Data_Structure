#define _CRT_SECURE_NO_WARNINGS
#define MAX_STUDENTS 100
#include <stdio.h>
#include <string.h>

struct Student {
    int id;
    char name[20];
    float score;
};

struct Class {
    struct Student stu[MAX_STUDENTS];
    int count;
} cls;

// 初始化班级人数
void InitClass(struct Class* cls) {
    cls->count = 0;
}

// 添加学生
int AddStudent(struct Class* cls, struct Student s) {
    if (cls->count >= MAX_STUDENTS) {
        return 1;
    }
    cls->stu[cls->count++] = s;
    return 0;
}

// 删除学生
int DeleteStudent(struct Class* cls, int id) {
    int i = 0;
    for (i = 0; i < cls->count; i++) {
        if (cls->stu[i].id == id) {
            for (int j = i; j < cls->count - 1; j++) {
                cls->stu[j] = cls->stu[j + 1];
            }
            cls->count--;
            return 0;
        }
    }
    return 1;
}

// 修改学生成绩
int UpdateScore(struct Class* cls, int id, float newScore) {
    int i = 0;
    for (i = 0; i < cls->count; i++) {
        if (cls->stu[i].id == id) {
            cls->stu[i].score = newScore;
            return 0;
        }
    }
    return 1;
}

// 按 id 查找学生，返回下标，找不到返回 -1
int FindStudent(struct Class* cls, int id) {
    for (int i = 0; i < cls->count; i++) {
        if (cls->stu[i].id == id) {
            return i;
        }
    }
    return -1;
}

// 打印学生信息
void PrintStudent(struct Class* cls) {
    if (cls->count == 0) {
        printf("当前没有学生记录。\n");
        return;
    }
    printf("id\tname\tscore\n");
    for (int i = 0; i < cls->count; i++) {
        printf("%d\t%s\t%.2f\n", cls->stu[i].id, cls->stu[i].name, cls->stu[i].score);
    }
}

// 显示菜单
void ShowMenu() {
    printf("\n========== 学生课程管理系统 ==========\n");
    printf("1. 添加学生\n");
    printf("2. 删除学生\n");
    printf("3. 修改学生成绩\n");
    printf("4. 打印全部学生\n");
    printf("5. 按 id 查找学生\n");
    printf("0. 退出\n");
    printf("=====================================\n");
    printf("请输入你的选择：");
}

int main() {
    struct Class cls;
    InitClass(&cls);

    int choice = -1;
    while (choice != 0) {
        ShowMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: { // 添加学生
            struct Student s;
            printf("请输入学生 id：");
            scanf("%d", &s.id);

            if (FindStudent(&cls, s.id) != -1) {
                printf("添加失败：id %d 已存在！\n", s.id);
                break;
            }

            printf("请输入学生姓名：");
            scanf("%s", s.name);
            printf("请输入学生成绩：");
            scanf("%f", &s.score);

            if (AddStudent(&cls, s) == 0) {
                printf("添加成功！\n");
            }
            else {
                printf("添加失败：人数已达上限！\n");
            }
            break;
        }

        case 2: { // 删除学生
            int id;
            printf("请输入要删除的学生 id：");
            scanf("%d", &id);

            if (DeleteStudent(&cls, id) == 0) {
                printf("删除成功！\n");
            }
            else {
                printf("删除失败：未找到 id 为 %d 的学生。\n", id);
            }
            break;
        }

        case 3: { // 修改成绩
            int id;
            float newScore;
            printf("请输入要修改的学生 id：");
            scanf("%d", &id);

            if (FindStudent(&cls, id) == -1) {
                printf("修改失败：未找到 id 为 %d 的学生。\n", id);
                break;
            }

            printf("请输入新的成绩：");
            scanf("%f", &newScore);

            if (UpdateScore(&cls, id, newScore) == 0) {
                printf("修改成功！\n");
            }
            else {
                printf("修改失败。\n");
            }
            break;
        }

        case 4: { // 打印全部
            PrintStudent(&cls);
            break;
        }

        case 5: { // 按 id 查找
            int id;
            printf("请输入要查找的学生 id：");
            scanf("%d", &id);

            int idx = FindStudent(&cls, id);
            if (idx == -1) {
                printf("未找到 id 为 %d 的学生。\n", id);
            }
            else {
                printf("id\tname\tscore\n");
                printf("%d\t%s\t%.2f\n",
                    cls.stu[idx].id, cls.stu[idx].name, cls.stu[idx].score);
            }
            break;
        }

        case 0: {
            printf("程序已退出。\n");
            break;
        }

        default: {
            printf("无效选择，请重新输入！\n");
            break;
        }
        }
    }

    return 0;
}