/*
 * 小型静态特征识别器——第一课：CSV 数据读取。
 * 本程序尚未完成：标有 TODO（待完成）的函数只是占位，暂不具备相应功能。
 * 本实验只处理教师提供的模拟数据，不读取、运行、下载或传播真实恶意程序。
 */
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FEATURE_COUNT 5
#define MAX_SAMPLES 512

 /* 单条样本：五项数值特征，加上一个类别标签（0或1）。 */
typedef struct {
    double features[FEATURE_COUNT];
    int label;
} Sample;

/* 数据集：用数组存放多条样本，count记录实际读取数量。 */
typedef struct {
    Sample samples[MAX_SAMPLES];
    int count;
} Dataset;

/* 归一化参数：每项特征的最小值和最大值。 */
typedef struct {
    double minv[FEATURE_COUNT];
    double maxv[FEATURE_COUNT];
} FeatureScaler;

/* 分类模型：五个特征权重、一个偏置，以及模型是否可用的标志。 */
typedef struct {
    double weights[FEATURE_COUNT];
    double bias;
    int trained;
} Model;

/* 读取带标签的 CSV 文件；其他模块留待后续实现。 */
static int load_dataset(const char* path, Dataset* data) {
    /* 尝试按照用户输入的路径打开CSV文件 */
    FILE* fp = fopen(path, "r");

    /* 打开失败时，输出系统提供的具体错误原因 */
    if (fp == NULL) {
        printf("尝试打开的文件：%s\n", path);
        perror("文件打开失败");
        return 1;
    }


    char line[1024];
    const char* header = "file_size,string_count,entropy,byte_00_ratio,byte_ff_ratio,label";
    /* 读取并严格核对表头，确保字段名称和顺序正确。 */
    if (!fgets(line, sizeof line, fp)) {
        puts("Error: empty CSV file."); fclose(fp); return 1;
    }
    if (!strchr(line, '\n') && !feof(fp)) {
        puts("Error: header too long."); fclose(fp); return 1;
    }
    line[strcspn(line, "\r\n")] = '\0';
    if (strcmp(line, header) != 0) {
        puts("Error: CSV header or column order is incorrect.");
        fclose(fp); return 1;
    }

    Dataset temp = { 0 };  /* 临时数据集：读取失败时不覆盖原来已加载的数据。 */
    int row = 1;
    /* 逐行读取数据，先解析到临时数据集。 */
    while (fgets(line, sizeof line, fp)) {
        row++;
        if (!strchr(line, '\n') && !feof(fp)) {
            printf("Error: row %d is too long.\n", row); fclose(fp); return 1;
        }
        if (temp.count >= MAX_SAMPLES) {
            puts("Error: sample limit exceeded."); fclose(fp); return 1;
        }
        /* 每行必须恰好包含六个用逗号分隔的数值字段。 */
        double values[6];
        char* cursor = line;
        for (int col = 0; col < 6; col++) {
            while (isspace((unsigned char)*cursor) && *cursor != '\n') cursor++;
            if (*cursor == ',' || *cursor == '\n' || *cursor == '\r' || *cursor == '\0') {
                printf("Error: missing field at row %d, column %d.\n", row, col + 1);
                fclose(fp); return 1;
            }
            errno = 0;
            char* end;
            /* 将文本转成double，并检查是否转换成功或数值溢出。 */
            values[col] = strtod(cursor, &end);
            if (end == cursor || errno == ERANGE || !isfinite(values[col])) {
                printf("Error: invalid number at row %d, column %d.\n", row, col + 1);
                fclose(fp); return 1;
            }
            while (*end == ' ' || *end == '\t') end++;
            if (col < 5) {
                if (*end != ',') {
                    printf("Error: column mismatch at row %d.\n", row);
                    fclose(fp); return 1;
                }
                cursor = end + 1;
            }
            else if (*end != '\n' && *end != '\r' && *end != '\0') {
                printf("Error: extra or invalid field at row %d.\n", row);
                fclose(fp); return 1;
            }
        }
        /* 检查大小、字符串数量、熵、两项比例及标签的取值范围。 */
        if (values[0] < 0 || values[1] < 0 || floor(values[1]) != values[1] ||
            values[2] < 0 || values[2] > 8 ||
            values[3] < 0 || values[3] > 1 ||
            values[4] < 0 || values[4] > 1 ||
            (values[5] != 0 && values[5] != 1)) {
            printf("Error: feature or label out of range at row %d.\n", row);
            fclose(fp); return 1;
        }
        Sample* s = &temp.samples[temp.count];
        for (int j = 0; j < FEATURE_COUNT; j++) s->features[j] = values[j];
        s->label = (int)values[5];
        temp.count++;
    }
    if (ferror(fp)) { puts("Error: file read failed."); fclose(fp); return 1; }
    fclose(fp);
    if (temp.count == 0) { puts("Error: no data rows."); return 1; }
    /* 全部读取成功后，才正式更新调用者的数据集。 */
    *data = temp;
    printf("Loaded %d samples successfully.\n", data->count);
    printf("First sample: file_size=%.0f, label=%d\n",
        data->samples[0].features[0], data->samples[0].label);
    return 0;
}

/* 待完成2：独立检查样本数量、标签、信息熵、字节比例等是否合法。 */

/* 第二课：检查已加载的数据集是否符合实验要求 */
static int validate_dataset(const Dataset* data) {

    /* 第一步：检查数据集指针和样本数量 */
    if (data == NULL) {
        printf("错误：数据集不存在。\n");
        return 1;
    }

    if (data->count <= 0 || data->count > MAX_SAMPLES) {
        printf("错误：数据集为空或样本数量超出限制。\n");
        return 1;
    }

    /* 第二步：逐条检查数据集中的样本 */
    for (int i = 0; i < data->count; i++) {

        /* s指向当前正在检查的样本 */
        const Sample* s = &data->samples[i];

        /* 检查标签是否只能为0或1 */
        if (s->label != 0 && s->label != 1) {
            printf("错误：第%d条样本标签非法。\n", i + 1);
            return 1;
        }

        /* 第三步：检查五项特征是否都是有限数值 */
        for (int j = 0; j < FEATURE_COUNT; j++) {
            if (!isfinite(s->features[j])) {
                printf("错误：第%d条样本第%d项特征不是有效数值。\n",
                    i + 1, j + 1);
                return 1;
            }
        }

        /* 第四步：检查文件大小不能为负数 */
        if (s->features[0] < 0) {
            printf("错误：第%d条样本文件大小非法。\n", i + 1);
            return 1;
        }

        /* 第五步：检查字符串数量必须是非负整数 */
        if (s->features[1] < 0 ||
            floor(s->features[1]) != s->features[1]) {
            printf("错误：第%d条样本字符串数量非法。\n", i + 1);
            return 1;
        }

        /* 第六步：检查信息熵必须在0到8之间 */
        if (s->features[2] < 0 || s->features[2] > 8) {
            printf("错误：第%d条样本信息熵非法。\n", i + 1);
            return 1;
        }

        /* 第七步：检查两个字节比例必须在0到1之间 */
        if (s->features[3] < 0 || s->features[3] > 1 ||
            s->features[4] < 0 || s->features[4] > 1) {
            printf("错误：第%d条样本字节比例非法。\n", i + 1);
            return 1;
        }
    }

    /* 所有样本都通过检查 */
    printf("校验通过！共检查%d条样本。\n", data->count);
    return 0;
}

/* 待完成3：只在训练集上计算归一化参数；后续数据须使用相同参数。 */
static void fit_scaler(const Dataset* data, FeatureScaler* scaler) {
    (void)data;
    (void)scaler;
    puts("TODO: implement training-only min/max fitting.");
}

/* 待完成4：计算预测概率，即 sigmoid(偏置 + 各特征与权重乘积之和)。 */
static double predict_probability(const Model* model,
    const FeatureScaler* scaler,
    const double raw_features[FEATURE_COUNT]) {
    (void)model;
    (void)scaler;
    (void)raw_features;
    puts("TODO: implement normalized logistic prediction.");
    return -1.0;
}

/* 待完成5：读取参考模型；后续还需实现权重、偏置和归一化参数的保存。 */
static int load_model(const char* path, Model* model, FeatureScaler* scaler) {
    (void)path;
    (void)model;
    (void)scaler;
    puts("TODO: load all model + scaler parameters.");
    return 1;
}

static void show_notice(void) {
    puts("Teaching use only: this program handles synthetic CSV static features,"
        " NOT executable files or real security judgments.");
}

/* 程序入口：循环显示菜单，依据输入调用相应功能。 */
int main(void) {
    Dataset data = { 0 };
    FeatureScaler scaler = { 0 };
    Model model = { 0 };
    char input[128];
    char path[256];
    show_notice();
    for (;;) {
        puts("\n1 Load labeled CSV | 2 Validate | 3 Load reference model"
            " | 4 Predict first loaded row | 5 Show notice | 0 Exit");
        printf("Select: ");
        if (!fgets(input, sizeof input, stdin)) break;
        int action = atoi(input);
        if (action == 0) break;
        if (action == 1 || action == 3) {
            printf("Path: ");
            if (!fgets(path, sizeof path, stdin)) break;
            path[strcspn(path, "\r\n")] = '\0';
        }
        switch (action) {
        case 1: (void)load_dataset(path, &data); break;
        case 2: (void)validate_dataset(&data); break;
        case 3: (void)load_model(path, &model, &scaler); break;
        case 4:
            if (data.count <= 0 || !model.trained) {
                puts("Load a dataset and model before prediction.");
            }
            else {
                printf("Probability: %.6f\n",
                    predict_probability(&model, &scaler, data.samples[0].features));
            }
            break;
        case 5: show_notice(); break;
        default: puts("Unknown menu selection."); break;
        }
    }
    /* 进阶模型训练时才需要调用 fit_scaler(&data, &scaler)。 */
    if (0) fit_scaler(&data, &scaler); /* 仅用于展示函数调用方式；条件为0，因此当前不会执行。 */
    return 0;
}
