#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>        /* abs */
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define MAX_LAND_NUM 5
#define INF INT_MAX

/* 平面图画布尺寸 */
#define MAP_ROWS 15
#define MAP_COLS 52

typedef struct {
    char name[100];
    char code;               /* 景点代号，如 'A' */
    char introduction[200];
    int  x, y;               /* 平面图坐标（画布中的列、行） */
} Landmark;

typedef struct {
    Landmark landmarks[MAX_LAND_NUM];
    int graph[MAX_LAND_NUM][MAX_LAND_NUM];
} CampusMap;

/* ============ 初始化：景点信息 + 坐标 + 路径 ============ */
void InitLandmark(CampusMap* map) {
    strcpy(map->landmarks[0].name, "图书馆");
    map->landmarks[0].code = 'A';
    strcpy(map->landmarks[0].introduction,
        "学校学习资源中心，提供安静的学习环境，藏书资源丰富。");
    map->landmarks[0].x = 4;   map->landmarks[0].y = 2;

    strcpy(map->landmarks[1].name, "恒大教学楼");
    map->landmarks[1].code = 'B';
    strcpy(map->landmarks[1].introduction,
        "学校教学场所，包含实验室、大阶梯教室等。");
    map->landmarks[1].x = 20;  map->landmarks[1].y = 2;

    strcpy(map->landmarks[2].name, "霍英东体育馆");
    map->landmarks[2].code = 'C';
    strcpy(map->landmarks[2].introduction,
        "学校锻炼场所，提供室内篮球场。");
    map->landmarks[2].x = 42;  map->landmarks[2].y = 2;

    strcpy(map->landmarks[3].name, "餐厅");
    map->landmarks[3].code = 'D';
    strcpy(map->landmarks[3].introduction,
        "学生用餐地点，提供多种菜肴和休闲区域。");
    map->landmarks[3].x = 20;  map->landmarks[3].y = 11;

    strcpy(map->landmarks[4].name, "宿舍楼");
    map->landmarks[4].code = 'E';
    strcpy(map->landmarks[4].introduction,
        "学生住宿区域，提供舒适的居住环境。");
    map->landmarks[4].x = 4;   map->landmarks[4].y = 11;

    /* 邻接矩阵：0 自己，INF 无边 */
    for (int i = 0; i < MAX_LAND_NUM; i++)
        for (int j = 0; j < MAX_LAND_NUM; j++)
            map->graph[i][j] = (i == j) ? 0 : INF;

    /* 无向边（双向赋值） */
    map->graph[0][1] = 5;   map->graph[1][0] = 5;   /* A-B 图书馆-教学楼 */
    map->graph[1][2] = 10;  map->graph[2][1] = 10;  /* B-C 教学楼-体育馆 */
    map->graph[0][4] = 3;   map->graph[4][0] = 3;   /* A-E 图书馆-宿舍   */
    map->graph[1][3] = 5;   map->graph[3][1] = 5;   /* B-D 教学楼-餐厅   */
    map->graph[4][3] = 20;  map->graph[3][4] = 20;  /* E-D 宿舍-餐厅     */
}

/* ============ 在字符画布上画直线 ============ */
void DrawLine(char canvas[MAP_ROWS][MAP_COLS],
    int x1, int y1, int x2, int y2, char c) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int steps = (dx > dy) ? dx : dy;
    if (steps == 0) return;

    for (int i = 0; i <= steps; i++) {
        int x = x1 + (x2 - x1) * i / steps;
        int y = y1 + (y2 - y1) * i / steps;
        if (x >= 0 && x < MAP_COLS - 1 && y >= 0 && y < MAP_ROWS) {
            if (canvas[y][x] == ' ')
                canvas[y][x] = c;
        }
    }
}

/* ============ 打印校园平面图 ============ */
void PrintCampusMap(CampusMap* map) {
    char canvas[MAP_ROWS][MAP_COLS];

    /* 清空画布 */
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS - 1; j++) canvas[i][j] = ' ';
        canvas[i][MAP_COLS - 1] = '\0';
    }

    /* ① 先画所有边 */
    for (int i = 0; i < MAX_LAND_NUM; i++) {
        for (int j = i + 1; j < MAX_LAND_NUM; j++) {
            if (map->graph[i][j] < INF) {
                int x1 = map->landmarks[i].x, y1 = map->landmarks[i].y;
                int x2 = map->landmarks[j].x, y2 = map->landmarks[j].y;
                char ch = (y1 == y2) ? '-' : ((x1 == x2) ? '|' : '*');
                DrawLine(canvas, x1, y1, x2, y2, ch);
            }
        }
    }

    /* ② 再画景点（覆盖在线上面，保证景点可见） */
    for (int i = 0; i < MAX_LAND_NUM; i++) {
        int x = map->landmarks[i].x, y = map->landmarks[i].y;
        canvas[y][x] = map->landmarks[i].code;
    }

    /* ③ 输出画布 */
    printf("\n======================= 校园平面图 =======================\n\n");
    for (int i = 0; i < MAP_ROWS; i++)
        printf("   %s\n", canvas[i]);

    /* ④ 图例 */
    printf("\n   图例：");
    for (int i = 0; i < MAX_LAND_NUM; i++)
        printf("[%c]%s  ", map->landmarks[i].code, map->landmarks[i].name);

    /* ⑤ 路径长度 */
    printf("\n\n   路径长度：");
    for (int i = 0; i < MAX_LAND_NUM; i++)
        for (int j = i + 1; j < MAX_LAND_NUM; j++)
            if (map->graph[i][j] < INF)
                printf("%c-%c=%d  ", map->landmarks[i].code,
                    map->landmarks[j].code, map->graph[i][j]);

    printf("\n==========================================================\n");
}

/* ============ 按代号查下标 ============ */
int FindIndexByCode(CampusMap* map, char code) {
    code = toupper((unsigned char)code);
    for (int i = 0; i < MAX_LAND_NUM; i++)
        if (map->landmarks[i].code == code) return i;
    return -1;
}

/* ============ Floyd 算法 ============ */
void Floyd(CampusMap* map,
    int dist[MAX_LAND_NUM][MAX_LAND_NUM],
    int path[MAX_LAND_NUM][MAX_LAND_NUM]) {
    for (int i = 0; i < MAX_LAND_NUM; i++) {
        for (int j = 0; j < MAX_LAND_NUM; j++) {
            dist[i][j] = map->graph[i][j];
            path[i][j] = (i != j && map->graph[i][j] < INF) ? i : -1;
        }
    }
    for (int k = 0; k < MAX_LAND_NUM; k++)
        for (int i = 0; i < MAX_LAND_NUM; i++)
            for (int j = 0; j < MAX_LAND_NUM; j++)
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];
                }
}

/* ============ 正序打印路径 ============ */
void PrintPath(CampusMap* map,
    int path[MAX_LAND_NUM][MAX_LAND_NUM],
    int start, int cur) {
    if (cur == start) {
        printf("%s(%c)", map->landmarks[cur].name, map->landmarks[cur].code);
        return;
    }
    int pre = path[start][cur];
    PrintPath(map, path, start, pre);
    printf(" -> %s(%c)", map->landmarks[cur].name, map->landmarks[cur].code);
}

/* ============ 功能1：景点信息咨询 ============ */
void AskLandmarkInfo(CampusMap* map) {
    char code;
    printf("请输入要查询的景点代号(A-E): ");
    scanf(" %c", &code);

    int idx = FindIndexByCode(map, code);
    if (idx == -1) { printf("无效的景点代号。\n"); return; }

    printf("\n【景点信息】\n");
    printf("  名称: %s\n", map->landmarks[idx].name);
    printf("  代号: %c\n", map->landmarks[idx].code);
    printf("  简介: %s\n", map->landmarks[idx].introduction);
}

/* ============ 功能2：问路查询 ============ */
void AskForDirections(CampusMap* map,
    int dist[MAX_LAND_NUM][MAX_LAND_NUM],
    int path[MAX_LAND_NUM][MAX_LAND_NUM]) {
    char sc, ec;
    printf("请输入起点代号(A-E): "); scanf(" %c", &sc);
    printf("请输入终点代号(A-E): "); scanf(" %c", &ec);

    int start = FindIndexByCode(map, sc);
    int end = FindIndexByCode(map, ec);

    if (start == -1 || end == -1) { printf("无效的景点代号。\n"); return; }

    if (dist[start][end] == INF) {
        printf("从 %s(%c) 到 %s(%c) 没有路径。\n",
            map->landmarks[start].name, map->landmarks[start].code,
            map->landmarks[end].name, map->landmarks[end].code);
        return;
    }

    printf("\n从 %s(%c) 到 %s(%c) 的最短路径长度为: %d\n",
        map->landmarks[start].name, map->landmarks[start].code,
        map->landmarks[end].name, map->landmarks[end].code,
        dist[start][end]);
    printf("路径: ");
    PrintPath(map, path, start, end);
    printf("\n");
}

/* ============ 主函数：循环菜单 ============ */
int main() {
    CampusMap map;
    InitLandmark(&map);

    int dist[MAX_LAND_NUM][MAX_LAND_NUM];
    int path[MAX_LAND_NUM][MAX_LAND_NUM];
    Floyd(&map, dist, path);

    printf("***************** 欢迎来到 JNU 校园导游 *****************\n");

    int choice;
    while (1) {
        printf("\n================ 校园导游系统 ================\n");
        printf("  1. 查看校园平面图\n");
        printf("  2. 景点信息咨询\n");
        printf("  3. 问路查询（任意两景点最短路径）\n");
        printf("  0. 退出\n");
        printf("请选择: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("请输入数字。\n");
            continue;
        }
        switch (choice) {
        case 1: PrintCampusMap(&map);   break;
        case 2: AskLandmarkInfo(&map);  break;
        case 3: AskForDirections(&map, dist, path); break;
        case 0: printf("再见！\n"); return 0;
        default: printf("无效选择，请重新输入。\n");
        }
    }
}