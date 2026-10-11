/*
 * 小型静态特征识别器——  一：CSV 数据读取。
 * 本程序尚未完成：标有 TODO（待完成）的函数只是占位，暂不具备相应功能。
 * 本实验只处理教师提供的模拟数据，不读取、运行、下载或传播真实恶意程序。
 */
#define _CRT_SECURE_NO_WARNINGS
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

/* 分别检查每个字段，明确指出错误原因 */

/* 文件大小不能为负数 */
        if (values[0] < 0) {
            printf("错误：第%d行 file_size 文件大小不能为负数。\n", row);
            fclose(fp);
            return 1;
        }

        /* 字符串数量必须是非负整数 */
        if (values[1] < 0 || floor(values[1]) != values[1]) {
            printf("错误：第%d行 string_count 必须为非负整数。\n", row);
            fclose(fp);
            return 1;
        }

        /* 信息熵必须位于0到8之间 */
        if (values[2] < 0 || values[2] > 8) {
            printf("错误：第%d行 entropy 信息熵超出[0,8]范围。\n", row);
            fclose(fp);
            return 1;
        }

        /* 0x00字节比例必须位于0到1之间 */
        if (values[3] < 0 || values[3] > 1) {
            printf("错误：第%d行 byte_00_ratio 超出[0,1]范围。\n", row);
            fclose(fp);
            return 1;
        }

        /* 0xFF字节比例必须位于0到1之间 */
        if (values[4] < 0 || values[4] > 1) {
            printf("错误：第%d行 byte_ff_ratio 超出[0,1]范围。\n", row);
            fclose(fp);
            return 1;
        }

        /* 类别标签只能是0或1 */
        if (values[5] != 0 && values[5] != 1) {
            printf("错误：第%d行 label 类别标签只能为0或1。\n", row);
            fclose(fp);
            return 1;
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

/* 2：独立检查样本数量、标签、信息熵、字节比例等是否合法。 */

/* 二：检查已加载的数据集是否符合实验要求 */
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

/* 3：只在训练集上计算归一化参数；后续数据须使用相同参数。 */

/* 根据训练集计算五项特征的最小值和最大值 */
static void fit_scaler(const Dataset* data, FeatureScaler* scaler) {

    /* 检查指针和数据集是否有效 */
    if (data == NULL || scaler == NULL ||
        data->count <= 0 || data->count > MAX_SAMPLES) {
        puts("错误：训练集无效，无法计算归一化参数。");
        return;
    }

    /* 初始化：使用第一条样本作为初始最小值和最大值 */
    for (int j = 0; j < FEATURE_COUNT; j++) {
        scaler->minv[j] = data->samples[0].features[j];
        scaler->maxv[j] = data->samples[0].features[j];
    }

    /* 遍历训练集，分别寻找五项特征的最大值和最小值 */
    for (int i = 1; i < data->count; i++) {
        for (int j = 0; j < FEATURE_COUNT; j++) {

            double x = data->samples[i].features[j];

            if (x < scaler->minv[j]) {
                scaler->minv[j] = x;
            }

            if (x > scaler->maxv[j]) {
                scaler->maxv[j] = x;
            }
        }
    }

    puts("训练集Min-Max归一化参数计算完成。");
}


/* 使用已计算好的Min-Max参数对数据集进行归一化 */
static int normalize_dataset(
    const Dataset* data,
    const FeatureScaler* scaler,
    Dataset* normalized) {

    /* 检查参数及样本数量 */
    if (data == NULL || scaler == NULL ||
        normalized == NULL ||
        data->count <= 0 || data->count > MAX_SAMPLES) {
        puts("错误：归一化输入参数无效。");
        return 1;
    }

    /* 复制数据集，避免修改原始样本 */
    *normalized = *data;

    /* 对每条样本的五项特征依次进行归一化 */
    for (int i = 0; i < data->count; i++) {
        for (int j = 0; j < FEATURE_COUNT; j++) {

            double min = scaler->minv[j];
            double max = scaler->maxv[j];
            double raw = data->samples[i].features[j];

            /* 避免无效数值或错误的归一化范围 */
            if (!isfinite(raw) || !isfinite(min) ||
                !isfinite(max) || max < min) {
                puts("错误：归一化参数或样本特征无效。");
                return 1;
            }

            /* 最大值与最小值相等时，该特征归一化为0 */
            if (max == min) {
                normalized->samples[i].features[j] = 0.0;
            }
            else {
                normalized->samples[i].features[j] =
                    (raw - min) / (max - min);
            }
        }
    }

    puts("数据集归一化完成。");
    return 0;
}


/* 4：计算预测概率，即 sigmoid(偏置 + 各特征与权重乘积之和)。 */

/* 四：使用逻辑回归计算样本的预测概率 */
static double predict_probability(
    const Model* model,
    const FeatureScaler* scaler,
    const double raw_features[FEATURE_COUNT]) {

    /* 第一步：检查模型、归一化参数和样本指针 */
    if (model == NULL || scaler == NULL ||
        raw_features == NULL) {
        printf("错误：预测参数无效。\n");
        return -1.0;
    }

    /* 检查模型是否已经成功加载 */
    if (model->trained != 1) {
        printf("错误：模型尚未加载。\n");
        return -1.0;
    }

    /* 第二步：使用偏置作为加权和的初始值 */
    double z = model->bias;

    /* 检查偏置是否为有限数值 */
    if (!isfinite(z)) {
        printf("错误：模型偏置无效。\n");
        return -1.0;
    }

    /* 第三步：依次处理五项特征 */
    for (int j = 0; j < FEATURE_COUNT; j++) {

        /* 读取第j项特征的原始值和归一化参数 */
        double raw = raw_features[j];
        double min = scaler->minv[j];
        double max = scaler->maxv[j];
        double weight = model->weights[j];

        /* 检查参与计算的数据是否有效 */
        if (!isfinite(raw) || !isfinite(min) ||
            !isfinite(max) || !isfinite(weight) ||
            max < min) {
            printf("错误：第%d项特征或模型参数无效。\n", j + 1);
            return -1.0;
        }

        /* 第四步：执行Min-Max归一化 */
        double x;

        if (max == min) {
            /* 最大值等于最小值时，约定归一化结果为0 */
            x = 0.0;
        }
        else {
            x = (raw - min) / (max - min);
        }

        /* 第五步：将该特征的加权结果累加到z */
        z += weight * x;
    }

    /* 检查最终加权和是否有效 */
    if (!isfinite(z)) {
        printf("错误：预测计算结果无效。\n");
        return -1.0;
    }

    /* 第六步：通过Sigmoid函数计算预测概率 */
    double p;

    if (z >= 0) {
        p = 1.0 / (1.0 + exp(-z));
    }
    else {
        /* 当z为很大的负数时，采用更稳定的计算方式 */
        double ez = exp(z);
        p = ez / (1.0 + ez);
    }

    /* 第七步：将预测概率返回给主函数 */
    return p;
}



/* 三：读取教师提供的分类模型和归一化参数 */
static int load_model(const char* path, Model* model,
    FeatureScaler* scaler) {

    /* 第一步：检查传入的指针是否有效 */
    if (path == NULL || model == NULL || scaler == NULL) {
        printf("错误：模型加载参数无效。\n");
        return 1;
    }

    /* 第二步：打开模型文件 */
    FILE* fp = fopen(path, "r");

    if (fp == NULL) {
        perror("模型文件打开失败");
        return 1;
    }

    /* 使用临时变量保存读取结果，避免失败时破坏原模型 */
    Model temp_model = { 0 };
    FeatureScaler temp_scaler = { 0 };

    char header[64];
    int count;

    /* 第三步：读取并检查模型格式标识 */
    if (fscanf(fp, "%63s", header) != 1 ||
        strcmp(header, "TEACHMODEL_V1") != 0) {
        printf("错误：模型文件格式不正确。\n");
        fclose(fp);
        return 1;
    }

    /* 第四步：读取特征数量，必须等于5 */
    if (fscanf(fp, "%d", &count) != 1 ||
        count != FEATURE_COUNT) {
        printf("错误：模型特征数量不正确。\n");
        fclose(fp);
        return 1;
    }

    /* 第五步：依次读取5个模型权重 */
    for (int i = 0; i < FEATURE_COUNT; i++) {
        if (fscanf(fp, "%lf", &temp_model.weights[i]) != 1 ||
            !isfinite(temp_model.weights[i])) {
            printf("错误：第%d个模型权重无效。\n", i + 1);
            fclose(fp);
            return 1;
        }
    }

    /* 第六步：读取模型偏置 */
    if (fscanf(fp, "%lf", &temp_model.bias) != 1 ||
        !isfinite(temp_model.bias)) {
        printf("错误：模型偏置无效。\n");
        fclose(fp);
        return 1;
    }

    /* 第七步：读取5项特征的最小值 */
    for (int i = 0; i < FEATURE_COUNT; i++) {
        if (fscanf(fp, "%lf", &temp_scaler.minv[i]) != 1 ||
            !isfinite(temp_scaler.minv[i])) {
            printf("错误：第%d项最小值无效。\n", i + 1);
            fclose(fp);
            return 1;
        }
    }

    /* 第八步：读取5项特征的最大值 */
    for (int i = 0; i < FEATURE_COUNT; i++) {
        if (fscanf(fp, "%lf", &temp_scaler.maxv[i]) != 1 ||
            !isfinite(temp_scaler.maxv[i]) ||
            temp_scaler.maxv[i] < temp_scaler.minv[i]) {
            printf("错误：第%d项最大值无效。\n", i + 1);
            fclose(fp);
            return 1;
        }
    }

    /* 第九步：确认文件末尾没有多余内容 */
    int ch;
    do {
        ch = fgetc(fp);
    } while (ch == ' ' || ch == '\t' ||
        ch == '\n' || ch == '\r');

    if (ch != EOF || ferror(fp)) {
        printf("错误：模型文件包含多余内容或读取失败。\n");
        fclose(fp);
        return 1;
    }

    fclose(fp);

    /* 第十步：确认读取成功后，再更新正式模型 */
    temp_model.trained = 1;
    *model = temp_model;
    *scaler = temp_scaler;

    printf("模型加载成功！\n");
    printf("特征数量：%d\n", FEATURE_COUNT);
    printf("模型偏置：%.6f\n", model->bias);
    printf("第一个权重：%.6f\n", model->weights[0]);

    return 0;
}

/* 5：将模型及归一化参数保存到文本文件 */
static int save_model(const char* path,
    const Model* model,
    const FeatureScaler* scaler) {

    /* 第一步：检查指针和模型状态 */
    if (path == NULL || model == NULL ||
        scaler == NULL || model->trained != 1) {
        printf("错误：模型尚未准备好，无法保存。\n");
        return 1;
    }

    /* 第二步：检查所有模型参数是否有效 */
    if (!isfinite(model->bias)) {
        printf("错误：模型偏置无效。\n");
        return 1;
    }

    for (int i = 0; i < FEATURE_COUNT; i++) {
        if (!isfinite(model->weights[i]) ||
            !isfinite(scaler->minv[i]) ||
            !isfinite(scaler->maxv[i]) ||
            scaler->maxv[i] < scaler->minv[i]) {
            printf("错误：第%d项模型参数无效。\n", i + 1);
            return 1;
        }
    }

    /* 第三步：创建用于保存模型的文件 */
    FILE* fp = fopen(path, "w");

    if (fp == NULL) {
        perror("模型保存失败");
        return 1;
    }

    /* 第四步：写入模型文件的格式标识和特征数量 */
    fprintf(fp, "TEACHMODEL_V1\n");
    fprintf(fp, "%d\n", FEATURE_COUNT);

    /* 第五步：写入五个模型权重 */
    for (int i = 0; i < FEATURE_COUNT; i++) {
        fprintf(fp, "%.17g%c", model->weights[i],
            i == FEATURE_COUNT - 1 ? '\n' : ' ');
    }

    /* 第六步：写入模型偏置 */
    fprintf(fp, "%.17g\n", model->bias);

    /* 第七步：写入五个特征最小值 */
    for (int i = 0; i < FEATURE_COUNT; i++) {
        fprintf(fp, "%.17g%c", scaler->minv[i],
            i == FEATURE_COUNT - 1 ? '\n' : ' ');
    }

    /* 第八步：写入五个特征最大值 */
    for (int i = 0; i < FEATURE_COUNT; i++) {
        fprintf(fp, "%.17g%c", scaler->maxv[i],
            i == FEATURE_COUNT - 1 ? '\n' : ' ');
    }

    /* 第九步：检查写入和关闭文件是否成功 */
    int write_failed = ferror(fp);
    int close_failed = fclose(fp);

    if (write_failed || close_failed != 0) {
        printf("错误：模型文件写入不完整。\n");
        return 1;
    }

    printf("模型保存成功：%s\n", path);
    return 0;
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


    /* ===== 归一化功能临时测试开始 ===== */

    /* 创建只有两条样本的模拟训练集 */
    Dataset train = { 0 };
    FeatureScaler test_scaler = { 0 };
    Dataset normalized = { 0 };

    train.count = 2;

    /* 第一条训练样本 */
    train.samples[0].features[0] = 100;
    train.samples[0].features[1] = 10;
    train.samples[0].features[2] = 2;
    train.samples[0].features[3] = 0.2;
    train.samples[0].features[4] = 0.1;
    train.samples[0].label = 0;

    /* 第二条训练样本 */
    train.samples[1].features[0] = 300;
    train.samples[1].features[1] = 30;
    train.samples[1].features[2] = 6;
    train.samples[1].features[3] = 0.8;
    train.samples[1].features[4] = 0.5;
    train.samples[1].label = 1;

    /* 计算训练集的Min-Max参数 */
    fit_scaler(&train, &test_scaler);

    /* 输出各项特征的最小值和最大值 */
    printf("\n===== Min-Max参数测试 =====\n");

    for (int j = 0; j < FEATURE_COUNT; j++) {
        printf("特征%d：Min=%.6f，Max=%.6f\n",
            j + 1,
            test_scaler.minv[j],
            test_scaler.maxv[j]);
    }

    /* 使用计算得到的参数进行归一化 */
    if (normalize_dataset(&train, &test_scaler, &normalized) == 0) {

        printf("\n===== 归一化结果测试 =====\n");

        for (int i = 0; i < normalized.count; i++) {
            printf("样本%d：", i + 1);

            for (int j = 0; j < FEATURE_COUNT; j++) {
                printf("%.6f ", normalized.samples[i].features[j]);
            }

            printf("\n");
        }
    }

    /* ===== 归一化功能临时测试结束 ===== */

    show_notice();
    for (;;) {
        /* 显示功能菜单，新增选项6用于保存模型 */
        puts("\n1 Load labeled CSV | 2 Validate | 3 Load reference model"
            " | 4 Predict first loaded row | 5 Show notice"
            " | 6 Save model | 0 Exit");
        printf("Select: ");
        if (!fgets(input, sizeof input, stdin)) break;
        int action = atoi(input);
        if (action == 0) break;
        /* 菜单1、3、6都需要用户输入文件路径 */
        if (action == 1 || action == 3 || action == 6) 
        {
            printf("Path: ");
            if (!fgets(path, sizeof path, stdin)) break;
            path[strcspn(path, "\r\n")] = '\0';
        }
        switch (action) {
        case 1: (void)load_dataset(path, &data); break;
        case 2: (void)validate_dataset(&data); break;
        

        case 3:
            /* 加载模型，检查是否成功 */
            if (load_model(path, &model, &scaler) != 0) {

                /* 加载失败后禁止继续使用旧模型 */
                model.trained = 0;

                /* 清空原有归一化参数 */
                memset(&scaler, 0, sizeof(scaler));

                printf("模型加载失败，当前模型不可用。\n");
            }
            break;


        case 4: {
            /* 检查数据集和模型是否已经准备好 */
            if (data.count <= 0 || !model.trained) {
                printf("错误：请先加载数据集和模型。\n");
                break;
            }

            /* 调用预测函数，计算第一条样本的概率 */
            double p = predict_probability(
                &model, &scaler, data.samples[0].features);

            /* 检查预测是否成功 */
            if (!isfinite(p) || p < 0.0 || p > 1.0) {
                printf("错误：预测失败。\n");
                break;
            }

            /* 以0.5为分类阈值，得到预测标签 */
            int predicted_label = (p >= 0.5) ? 1 : 0;

            /* 输出完整的分类结果 */
            printf("\n========== 分类预测结果 ==========\n");
            printf("样本编号：1\n");
            /* 输出17位有效数字，便于高精度比较 */
            printf("预测概率：%.17g\n", p);
            printf("预测类别：%d\n", predicted_label);
            printf("类别说明：%s\n",
                predicted_label == 1 ? "可疑" : "良性");
            printf("原始标签：%d\n", data.samples[0].label);
            printf("==================================\n");

            break;
        }

        case 5:
            /* 显示教学安全说明 */
            show_notice();
            break;

        case 6:
            /* 将已加载的模型及归一化参数保存到指定文件 */
            (void)save_model(path, &model, &scaler);
            break;

        default:
            /* 用户输入了菜单中不存在的选项 */
            puts("Unknown menu selection.");
            break;
        }
    }

    return 0;
}
