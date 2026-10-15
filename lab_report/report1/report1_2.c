#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

typedef int Status;

typedef struct
{
    int fenzi;
    int fenmu;
} Rational; 
// 辅助函数求最大公约数 
int gcd(int a, int b)
{
    a = abs(a);
    b = abs(b);
    // 正确的辗转相除法
    while (b != 0) {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return a; // 必须要有返回值
}

// 辅助函数化简分数
void Simplify(Rational* r)
{
    if (r->fenmu == 0) return;

    int gcd1 = gcd(r->fenzi, r->fenmu);
    if (gcd1 > 0)
    {
        r->fenzi /= gcd1;
        r->fenmu /= gcd1;
    }

    // 统一将负号放在分子上
    if (r->fenmu < 0)
    {
        r->fenzi = -r->fenzi; 
        r->fenmu = -r->fenmu;
    }
}

Status InitRational(Rational* R, int num, int den)
{
    if (den == 0) {
        return ERROR;
    }
    R->fenzi = num;
    R->fenmu = den;
    Simplify(R);
    return OK;
}

// 加法：A + B
Rational Add(Rational A, Rational B) {
    Rational result;
    result.fenzi = A.fenzi * B.fenmu + B.fenzi * A.fenmu;
    result.fenmu = A.fenmu * B.fenmu;
    Simplify(&result);
    return result;
}

// 减法：A - B
Rational Subtract(Rational A, Rational B) {
    Rational result;
    result.fenzi = A.fenzi * B.fenmu - B.fenzi * A.fenmu;
    result.fenmu = A.fenmu * B.fenmu;
    Simplify(&result);
    return result;
}

// 乘法：A * B
Rational Multiply(Rational A, Rational B) {
    Rational result;
    result.fenzi = A.fenzi * B.fenzi;
    result.fenmu = A.fenmu * B.fenmu;
    Simplify(&result);
    return result;
}

// 除法 (选做)：A / B
Status Divide(Rational A, Rational B, Rational* result) {
    if (B.fenzi == 0) {
        return ERROR; // 除数不能为0
    }
    result->fenzi = A.fenzi * B.fenmu;
    result->fenmu = A.fenmu * B.fenzi;
    Simplify(result);
    return OK;
}

// 打印分数
void PrintRational(Rational R) {
    if (R.fenmu == 1) {
        printf("%d", R.fenzi);
    }
    else {
        printf("%d/%d", R.fenzi, R.fenmu);
    }
}

// ---------- 7. 主函数：测试代码 ----------
int main() {
    Rational A, B, res; 

    // 测试数据：A = 1/2, B = 3/4
    InitRational(&A, 1, 2);
    InitRational(&B, 3, 4);

    printf("测试数据：A = ");
    PrintRational(A);
    printf(", B = ");
    PrintRational(B);
    printf("\n\n");

    // 测试加法
    res = Add(A, B);
    printf("加法 A + B = ");
    PrintRational(res);
    printf("\n");

    // 测试减法
    res = Subtract(A, B);
    printf("减法 A - B = ");
    PrintRational(res);
    printf("\n");

    // 测试乘法
    res = Multiply(A, B);
    printf("乘法 A * B = ");
    PrintRational(res);
    printf("\n");

    // 测试除法 (选做)
    if (Divide(A, B, &res) == OK) {
        printf("除法 A / B = ");
        PrintRational(res);
        printf("\n");
    }
    else {
        printf("除法错误：除数不能为0\n");
    }
    printf("\n--- 测试分母为0的拦截情况 ---\n");
    Rational C;

    // 故意传入分母为 0，并用变量接收返回状态
    Status status = InitRational(&C, 5, 0);

    if (status == ERROR) {
        printf("测试通过：分母为0被成功拦截，初始化失败 (返回 ERROR)。\n");
    }
    else {
       
        printf("错误数据 C = ");
        PrintRational(C);
        printf("\n");
    }
    return 0;
}