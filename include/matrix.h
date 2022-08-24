/*******************************************************************************
 *       File Name :                         matrix.h
 *       Library/Module Name :               MatrixComputation
 *       Author :                            Marc Pony(marc_pony@163.com)
 *       Change by :                         J.L. Wang
 *       Abstract Description :              矩阵运算库头文件
 *       Rule :                              1. 函数名小写
 *                                           2. 结构体类型大写，多个单词中间用 _ 连接
 *                                           3. typedef定义的数据类型大写，多个单词中间用 _ 连接
 *******************************************************************************/

#ifndef MAIN_MATRIX_H
#define MAIN_MATRIX_H

/*******************************************************************************
 * (1)Debug Switch Section
 *******************************************************************************/

/*******************************************************************************
 * (2)Include File Section
 *******************************************************************************/
#include "base.h"

/*******************************************************************************
 * (3)Macro Define Section
 *******************************************************************************/

/*******************************************************************************
 * (4)Struct(Enum) Define Section
 *******************************************************************************/
/**********************************************************************************************
Description: 描述矩阵计算时是按行、按列还是全部（求均值等）
Value:  Row         合并行计算
        Columns     合并列计算
        All         合并所有数据计算
***********************************************************************************************/
#define CALCULATE_TAB          \
    CALCULATE_MACRO(Row, 0)    \
    CALCULATE_MACRO(Column, 1) \
    CALCULATE_MACRO(All, 2)

typedef enum
{
#define CALCULATE_MACRO(a, b) a = b,
    CALCULATE_TAB
#undef CALCULATE_MACRO
} CALCULATE_CHOOSE;

/**********************************************************************************************
Description: 矩阵结构体
Value:  rows        矩阵行数
        columns     矩阵列数
        p           指向矩阵储存的数据（申请的是一维空间）
***********************************************************************************************/
typedef struct matrix
{
    INTEGER rows;
    INTEGER columns;
    REAL *p;
} MATRIX;

/**********************************************************************************************
Description: 矩阵链表（不存储矩阵数据）
Value:  ptr         指向矩阵结构体的指针
        next        指向链表的下一个节点
***********************************************************************************************/
typedef struct matrix_node
{
    MATRIX *ptr;
    struct matrix_node *next;
} MATRIX_NODE;

/**********************************************************************************************
Description: 矩阵数据链表（存储矩阵数据）
Value:  ptr         指向矩阵数据的指针
        next        指向链表的下一个节点
***********************************************************************************************/
typedef struct matrix_element_node
{
    REAL *ptr;
    struct matrix_element_node *next;
} MATRIX_ELEMENT_NODE;

/**********************************************************************************************
Description: 矩阵栈（存储最后一个矩阵和矩阵数据节点的指针，矩阵和数据节点以头插法的方式加入到栈中）
Value:  matrixNode          指向矩阵节点
        matrixElementNode   指向矩阵数据节点
***********************************************************************************************/
typedef struct stacks
{
    MATRIX_NODE *matrixNode;
    MATRIX_ELEMENT_NODE *matrixElementNode;

    // ...
    // 添加其他对象的指针
} STACKS;

/*******************************************************************************
 * (5)Prototype Declare Section
 *******************************************************************************/
/**********************************************************************************************
Function: init_stack
Description: 初始化栈
Input: 无
Output: 无
Input_Output: 栈指针
Return: 无
***********************************************************************************************/
VOID init_stack(_IN_OUT STACKS *S);

/**********************************************************************************************
Function: free_stack
Description: 释放栈
Input: 栈指针
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID free_stack(_IN STACKS *S);

/**********************************************************************************************
Function: print_matrix
Description: 输出矩阵数据
Input: 矩阵A，名称name
Output: 无
Input_Output: 无
Return: 无
***********************************************************************************************/
VOID print_matrix(MATRIX *a, NAME name);

/**********************************************************************************************
Function: creat_matrix
Description: 创建矩阵
Input: 矩阵行数rows，列数columns
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *creat_matrix(_IN INTEGER rows, _IN INTEGER columns, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: creat_multiple_matrices
Description: 创建多个矩阵
Input: 矩阵行数rows，列数columns，个数count
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *creat_multiple_matrices(_IN INTEGER rows, _IN INTEGER columns, _IN INTEGER count, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: read_matrix_from_csv
Description: 从CSV文件中读取矩阵（默认第一行为表头)
Input: 文件名称filename
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *read_matrix_from_csv(_IN NAME filename, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: creat_zero_matrix
Description: 创建零矩阵
Input: 矩阵行数rows，列数columns
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *creat_zero_matrix(_IN INTEGER rows, _IN INTEGER columns, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: creat_eye_matrix
Description: 创建单位矩阵
Input: 矩阵行数rows，列数columns
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *creat_eye_matrix(_IN INTEGER n, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: matrix_cut
Description: 矩阵剪切
Input: 矩阵A，选择choose(Row/Column)，初始列st，结束列ed
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_cut(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _IN INTEGER st, _IN INTEGER ed, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: matrix_sum
Description: 矩阵求和
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_sum(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: matrix_average
Description: 矩阵求均值
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_average(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: matrix_average_broadcast
Description: 矩阵求均值再广播到原矩阵
Input: 矩阵A，选择choose(Row/Column/All)
Output: 无
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
ERROR_ID matrix_average_broadcast(_IN_OUT MATRIX *A, _IN CALCULATE_CHOOSE choose);

/**********************************************************************************************
Function: matrix_min
Description: 矩阵求最小值
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_min(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: matrix_max
Description: 矩阵求最大值
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_max(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: matrix_var
Description: 矩阵求方差
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_var(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: matrix_std
Description: 矩阵求标准差
Input: 矩阵A，选择choose(Row/Column/All)
Output: 错误号指针errorID，栈指针S
Input_Output: 无
Return: 矩阵指针
***********************************************************************************************/
MATRIX *matrix_std(_IN MATRIX *A, _IN CALCULATE_CHOOSE choose, _OUT ERROR_ID *errorID, _OUT STACKS *S);

/**********************************************************************************************
Function: matrix_add
Description: 矩阵A + 矩阵B = 矩阵C
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_add(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C);

/**********************************************************************************************
Function: matrix_sub
Description: 矩阵A - 矩阵B = 矩阵C
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_sub(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C);

/**********************************************************************************************
Function: matrix_mul
Description: 矩阵乘法C = A * B
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_mul(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C);

/**********************************************************************************************
Function: matrix_dot
Description: 矩阵对应位置相乘C = A dot B
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_dot(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C);

/**********************************************************************************************
Function: matrix_divide
Description: 矩阵对应位置（！！！）相除C = A / B
Input: 矩阵A,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_divide(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C);

/**********************************************************************************************
Function: matrix_add_num
Description: 矩阵乘法B = A + a
Input: 矩阵A,矩阵B
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_add_num(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B);

/**********************************************************************************************
Function: matrix_sub_num
Description: 矩阵乘法B = A - a
Input: 矩阵A,矩阵B
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_sub_num(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B);

/**********************************************************************************************
Function: matrix_mul_num
Description: 矩阵乘法B = A * a
Input: 矩阵A,矩阵B
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_mul_num(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B);

/**********************************************************************************************
Function: matrix_pow_num
Description: 矩阵的幂B = A ^ a
Input: 矩阵A,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_pow_num(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B);

/**********************************************************************************************
Function: num_divide_matrix
Description: 数除矩阵B = a / A
Input: 矩阵A,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID num_divide_matrix(_IN REAL a, _IN MATRIX *A, _OUT MATRIX *B);

/**********************************************************************************************
Function: matrix_inverse
Description: 矩阵求逆
Input: 矩阵A
Output: 矩阵A的逆矩阵
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_inverse(_IN MATRIX *A, _OUT MATRIX *invA);

/**********************************************************************************************
Function: matrix_transpose
Description: 矩阵转置
Input: 矩阵A
Output: 矩阵A的转置
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_transpose(_IN MATRIX *A, _OUT MATRIX *transposeA);

/**********************************************************************************************
Function: matrix_trace
Description: 矩阵的迹
Input: 矩阵A
Output: 矩阵A的迹
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID matrix_trace(_IN MATRIX *A, _OUT REAL *trace);

/**********************************************************************************************
Function: lup_decomposition
Description: n行n列矩阵LUP分解PA = L * U
Input: n行n列矩阵A
Output: n行n列下三角矩阵L，n行n列上三角矩阵U，n行n列置换矩阵P
Input_Output: 无
Return: 错误号
***********************************************************************************************/
ERROR_ID lup_decomposition(_IN MATRIX *A, _OUT MATRIX *L, _OUT MATRIX *U, _OUT MATRIX *P);

/**********************************************************************************************
Function: solve_matrix_equation_by_lup_decomposition
Description: LUP分解解矩阵方程AX=B,其中A为n行n列矩阵，B为n行m列矩阵，X为n行m列待求矩阵(写到矩阵B)
Input: n行n列矩阵A
Output: 无
Input_Output: n行m列矩阵B(即n行m列待求矩阵X)
Return: 错误号
***********************************************************************************************/
ERROR_ID solve_matrix_equation_by_lup_decomposition(_IN MATRIX *A, _IN_OUT MATRIX *B);

/*******************************************************************************
 * Data Types Define Section
 *******************************************************************************/
/**********************************************************************************************
Function: matrix_matrix_calculate
Description: 矩阵计算C = A + B or A - B or A * B
Input: 矩阵A,函数指针fp,矩阵B
Output: 矩阵C
Input_Output: 无
Return: 错误号
***********************************************************************************************/
typedef ERROR_ID (*PTR_TO_MATRIX_MATRIX_CALCULATE)(_IN MATRIX *A, _IN MATRIX *B, _OUT MATRIX *C);
ERROR_ID matrix_matrix_calculate(_IN MATRIX *A, _IN PTR_TO_MATRIX_MATRIX_CALCULATE fp, _IN MATRIX *B, _OUT MATRIX *C);

/**********************************************************************************************
Function: matrix_num_calculate
Description: 矩阵与数计算C = A + a or A - a or A * a
Input: 矩阵A,函数指针fp,实数a
Output: 矩阵B
Input_Output: 无
Return: 错误号
***********************************************************************************************/
typedef ERROR_ID (*PTR_TO_MATRIX_NUM_CALCULATE)(_IN MATRIX *A, _IN REAL a, _OUT MATRIX *B);
ERROR_ID matrix_num_calculate(_IN MATRIX *A, _IN PTR_TO_MATRIX_NUM_CALCULATE fp, _IN REAL a, _OUT MATRIX *B);
#endif // MAIN_MATRIX_H
