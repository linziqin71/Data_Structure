#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define V 6   /* 题目要求：6 个结点 v0 ~ v5 */

/* 在"还没确定的点"里，找出当前距离最小的那个点的下标；找不到返回 -1 */
int shortestDist(int dist[], bool final[]) {
    int short_index = -1;
    int min = INT_MAX;
    for (int i = 0; i < V; i++) {
        if (!final[i] && dist[i] < min) {
            min = dist[i];
            short_index = i;
        }
    }
    return short_index;
}

/* 打印结果：每个点的最短距离 + 路线 */
void printDist(int dist[], int path[V][V]) {
    printf("顶点\t距离\t路线\n");
    for (int i = 0; i < V; i++) {
        printf("v%d\t", i);
        if (dist[i] == INT_MAX) printf("∞\t");   /* 到不了 */
        else                    printf("%d\t", dist[i]);

        /* path[i] 里存的是 "v0 -> ... -> vi" 这条路线 */
        for (int k = 0; k < V; k++) {
            if (k > 0) printf(" -> ");
            printf("v%d", path[i][k]);
            if (path[i][k] == i) break;           /* 走到终点 i 就停 */
        }
        printf("\n");
    }
}

/* Dijkstra：求源点 start 到其它所有点的最短路径 */
void Dijkstra(int graph[V][V], int start) {
    int  dist[V];          /* dist[i] = 当前已知的 start 到 i 的最短距离 */
    bool final[V];         /* final[i] = true 表示 i 的最短路径已确定   */
    int  path[V][V] = { 0 }; /* path[i] 记录路线，比如 {0,3,4} = v0->v3->v4 */

    /* ===== 1. 初始化 ===== */
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;   /* 距离先当"无穷远" */
        final[i] = false;     /* 都没确定 */
        path[i][0] = i;         /* 路线暂时只含自己 */
    }
    dist[start] = 0;            /* 起点到自己 = 0 */

    /* ===== 2. 主循环：一共挑 V 次，第一次挑出的就是起点 ===== */
    for (int i = 0; i < V; i++) {
        int u = shortestDist(dist, final);   /* 挑当前距离最小的点 */
        if (u == -1) break;                  /* 剩下的都到不了，退出 */
        final[u] = true;                     /* 确定它 */

        /* ===== 3. 用 u 去更新它能直接到的邻居 ===== */
        for (int a = 0; a < V; a++) {
            if (!final[a] && graph[u][a] != 0 && dist[u] != INT_MAX &&
                dist[a] > dist[u] + graph[u][a]) {

                dist[a] = dist[u] + graph[u][a];   /* 发现更短，更新距离 */

                /* 顺便更新路线：把到 u 的路线抄过来，末尾加上 a */
                for (int k = 0; k < V; k++) {
                    path[a][k] = path[u][k];
                    if (path[u][k] == u) {
                        path[a][k + 1] = a;
                        break;
                    }
                }
            }
        }
    }

    printDist(dist, path);   /* 输出结果 */
}

int main() {
    /* 6 个结点的有向网（0 = 两点之间没有直接的路） */
    /*          v0  v1  v2  v3  v4  v5  */
    int graph[V][V] = {
        { 0,  6,  0,  1,  0, 40 },  /* v0：到 v1 长 6，到 v3 长 1，到 v5 长 40 */
        { 0,  0, 43, 11,  6,  0 },  /* v1 */
        { 0,  0,  0,  0,  0,  8 },  /* v2 */
        { 0,  0,  0,  0,  1,  0 },  /* v3 */
        { 0,  0, 38,  0,  0,  0 },  /* v4 */
        { 0,  0,  0,  0,  5,  0 }   /* v5 */
    };

    Dijkstra(graph, 0);   /* 求 v0 到其它点的最短路径 */
    return 0;
}